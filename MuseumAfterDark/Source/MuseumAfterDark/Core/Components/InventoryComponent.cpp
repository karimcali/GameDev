#include "MuseumAfterDark/Core/Components/InventoryComponent.h"
#include "Engine/DataTable.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	MaxSlots = 100;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.SetNum(MaxSlots);
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
	UE_LOG(LogTemp, Warning, TEXT("AddItem called: ItemID=%s, Quantity=%d"), *ItemID.ToString(), Quantity);
	
	if (ItemID == NAME_None || Quantity <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("AddItem failed: Invalid ItemID or Quantity"));
		return false;
	}

	if (!ItemsDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("AddItem failed: ItemsDataTable is NULL! Check BP_MuseumPlayer -> Inventory Component -> Items Data Table"));
		return false;
	}

	FItemData ItemData;
	if (!GetItemData(ItemID, ItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("AddItem failed: ItemID %s not found in Data Table"), *ItemID.ToString());
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("AddItem: Found item data for %s"), *ItemID.ToString());

	// If item is stackable, try to add to existing stack
	if (ItemData.bStackable)
	{
		int32 ExistingSlotIndex = FindItemSlot(ItemID);
		if (ExistingSlotIndex != INDEX_NONE)
		{
			FInventorySlot& Slot = InventorySlots[ExistingSlotIndex];
			int32 NewQuantity = FMath::Min(Slot.Quantity + Quantity, ItemData.MaxStack);
			int32 Added = NewQuantity - Slot.Quantity;
			Slot.Quantity = NewQuantity;
			
			if (Added > 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("AddItem SUCCESS: Added %d to existing stack of %s (Total: %d)"), Added, *ItemID.ToString(), Slot.Quantity);
				OnInventoryChanged.Broadcast();
				
				// Check for fragment combination
				if (!ItemData.CombineGroupID.IsNone())
				{
					CombineFragments(ItemData.CombineGroupID);
				}
				
				return true;
			}
		}
	}

	// Find empty slot for new item
	int32 EmptySlotIndex = FindEmptySlot();
	if (EmptySlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full"));
		return false;
	}

	InventorySlots[EmptySlotIndex] = FInventorySlot(ItemID, Quantity);
	UE_LOG(LogTemp, Warning, TEXT("AddItem SUCCESS: Added %s (Quantity: %d) to slot %d"), *ItemID.ToString(), Quantity, EmptySlotIndex);
	OnInventoryChanged.Broadcast();

	// Check for fragment combination
	if (!ItemData.CombineGroupID.IsNone())
	{
		CombineFragments(ItemData.CombineGroupID);
	}

	return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
	if (ItemID == NAME_None || Quantity <= 0)
	{
		return false;
	}

	int32 SlotIndex = FindItemSlot(ItemID);
	if (SlotIndex == INDEX_NONE)
	{
		return false;
	}

	FInventorySlot& Slot = InventorySlots[SlotIndex];
	if (Slot.Quantity <= Quantity)
	{
		Slot = FInventorySlot(); // Clear slot
	}
	else
	{
		Slot.Quantity -= Quantity;
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
	int32 SlotIndex = FindItemSlot(ItemID);
	if (SlotIndex == INDEX_NONE)
	{
		return false;
	}

	return InventorySlots[SlotIndex].Quantity >= Quantity;
}

int32 UInventoryComponent::GetItemQuantity(FName ItemID) const
{
	int32 SlotIndex = FindItemSlot(ItemID);
	if (SlotIndex == INDEX_NONE)
	{
		return 0;
	}

	return InventorySlots[SlotIndex].Quantity;
}

bool UInventoryComponent::HasTag(EItemTag Tag) const
{
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}

		FItemData ItemData;
		if (GetItemData(Slot.ItemID, ItemData))
		{
			if (ItemData.ItemTags.Contains(Tag))
			{
				return true;
			}
		}
	}

	return false;
}

bool UInventoryComponent::CombineFragments(FName CombineGroupID)
{
	if (CombineGroupID.IsNone())
	{
		return false;
	}

	// Find all fragments with this CombineGroupID
	TArray<FName> FragmentItemIDs;
	
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			continue;
		}

		FItemData ItemData;
		if (GetItemData(Slot.ItemID, ItemData))
		{
			if (ItemData.CombineGroupID == CombineGroupID)
			{
				FragmentItemIDs.AddUnique(Slot.ItemID);
			}
		}
	}

	// Check if we have all fragments (assuming 4 fragments per group)
	if (FragmentItemIDs.Num() >= 4)
	{
		// Remove all fragments
		for (FName FragmentID : FragmentItemIDs)
		{
			RemoveItem(FragmentID, 1);
		}

		// Add combined item (you'll need to define what the combined item is)
		UE_LOG(LogTemp, Log, TEXT("Combined fragments from group: %s"), *CombineGroupID.ToString());
		OnInventoryChanged.Broadcast();
		return true;
	}

	return false;
}

bool UInventoryComponent::GetItemData(FName ItemID, FItemData& OutItemData) const
{
	if (!ItemsDataTable || ItemID == NAME_None)
	{
		return false;
	}

	FItemData* FoundRow = ItemsDataTable->FindRow<FItemData>(ItemID, TEXT("GetItemData"));
	if (FoundRow)
	{
		OutItemData = *FoundRow;
		return true;
	}

	return false;
}

int32 UInventoryComponent::FindItemSlot(FName ItemID) const
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].ItemID == ItemID && !InventorySlots[i].IsEmpty())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 UInventoryComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].IsEmpty())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

