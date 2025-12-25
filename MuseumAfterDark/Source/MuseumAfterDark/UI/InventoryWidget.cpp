#include "MuseumAfterDark/UI/InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "MuseumAfterDark/Core/Components/InventoryComponent.h"
#include "MuseumAfterDark/Core/Structs/ItemData.h"
#include "Engine/DataTable.h"
#include "Slate/SlateBrushAsset.h"
#include "Styling/SlateBrush.h"
#include "GameFramework/PlayerController.h"
#include "Framework/Application/SlateApplication.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Initialize selection
	SelectedItemID = NAME_None;

	// Hide by default
	HideInventory();
}

void UInventoryWidget::InitializeInventory(UInventoryComponent* InInventoryComponent)
{
	if (!InInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: InitializeInventory called with NULL InventoryComponent"));
		return;
	}

	InventoryComponent = InInventoryComponent;

	// Bind to inventory changed delegate
	if (InventoryComponent)
	{
		InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::OnInventoryChanged);
	}

	// Initial refresh
	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: RefreshInventory called but InventoryComponent is NULL"));
		return;
	}

	if (!InventoryGrid)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: InventoryGrid is NULL - make sure it's bound in Blueprint"));
		return;
	}

	// Get all inventory slots
	TArray<FInventorySlot> AllSlots = InventoryComponent->GetInventorySlots();
	TArray<FInventorySlot> NonEmptySlots;

	// Filter out empty slots
	for (const FInventorySlot& InvSlot : AllSlots)
	{
		if (!InvSlot.IsEmpty())
		{
			NonEmptySlots.Add(InvSlot);
		}
	}

	// Clear existing slots
	InventoryGrid->ClearChildren();

	// Create widgets for all items (max 18)
	UE_LOG(LogTemp, Log, TEXT("InventoryWidget: Creating %d item slots"), NonEmptySlots.Num());
	int32 GridIndex = 0;
	for (int32 i = 0; i < NonEmptySlots.Num() && GridIndex < MaxItemsDisplayed; ++i)
	{
		const FInventorySlot& InvSlot = NonEmptySlots[i];
		UE_LOG(LogTemp, Log, TEXT("InventoryWidget: Creating slot for item %s"), *InvSlot.ItemID.ToString());
		CreateSlotWidget(InvSlot, i, GridIndex);
		GridIndex++;
	}

	// Update details panel if item is still visible
	if (SelectedItemID != NAME_None)
	{
		bool bStillVisible = false;
		for (const FInventorySlot& InvSlot : NonEmptySlots)
		{
			if (InvSlot.ItemID == SelectedItemID)
			{
				bStillVisible = true;
				break;
			}
		}

		if (!bStillVisible)
		{
			SelectedItemID = NAME_None;
			UpdateDetailsPanel(NAME_None);
		}
		else
		{
			// Refresh details panel
			UpdateDetailsPanel(SelectedItemID);
		}
	}
}

void UInventoryWidget::CreateSlotWidget(const FInventorySlot& InvSlot, int32 SlotIndex, int32 GridIndex)
{
	if (!InventoryGrid)
	{
		return;
	}

	// Get item data
	FItemData ItemData;
	if (!InventoryComponent || !InventoryComponent->GetItemData(InvSlot.ItemID, ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Could not get item data for %s"), *InvSlot.ItemID.ToString());
		return;
	}

	// Create border wrapper for slot (gives it a background and border)
	UBorder* SlotBorder = NewObject<UBorder>(this);
	if (!SlotBorder)
	{
		return;
	}

	// Set border background color
	if (SelectedItemID == InvSlot.ItemID)
	{
		// Dark blue-gray background when selected
		SlotBorder->SetBrushColor(FLinearColor(0.2f, 0.2f, 0.3f, 1.0f));
	}
	else
	{
		// Dark gray background when not selected
		SlotBorder->SetBrushColor(FLinearColor(0.15f, 0.15f, 0.15f, 1.0f));
	}
	
	// Set border padding (creates space inside border)
	FMargin BorderPadding(2.0f);
	SlotBorder->SetPadding(BorderPadding);

	// Create button for slot (clickable container)
	UButton* SlotButton = NewObject<UButton>(this);
	if (!SlotButton)
	{
		return;
	}

	// Set button to be transparent (border provides the visual)
	FButtonStyle ButtonStyle = SlotButton->GetStyle();
	ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor::White);
	ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.2f, 1.2f, 1.2f, 1.0f)); // Slight brighten on hover
	ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.9f, 0.9f, 0.9f, 1.0f)); // Slight darken on press
	SlotButton->SetStyle(ButtonStyle);

	// Create image for item icon
	UImage* IconImage = NewObject<UImage>(this);
	if (IconImage)
	{
		// Load the icon texture
		if (!ItemData.Icon.IsNull())
		{
			UTexture2D* IconTexture = ItemData.Icon.LoadSynchronous();
			if (IconTexture)
			{
				IconImage->SetBrushFromTexture(IconTexture);
				IconImage->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f)); // Slightly smaller to fit in border
				UE_LOG(LogTemp, Log, TEXT("InventoryWidget: Loaded icon texture for %s"), *InvSlot.ItemID.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Failed to load icon texture for %s"), *InvSlot.ItemID.ToString());
				IconImage->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
				IconImage->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: No icon set for item %s"), *InvSlot.ItemID.ToString());
			// Placeholder if no icon - show a question mark or gray box
			IconImage->SetColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f));
			IconImage->SetDesiredSizeOverride(FVector2D(90.0f, 90.0f));
		}

		IconImage->SetVisibility(ESlateVisibility::Visible);
		SlotButton->SetContent(IconImage);
	}

	// Put button inside border
	SlotBorder->SetContent(SlotButton);

	// Bind click handler using lambda that captures ItemID
	FName CapturedItemID = InvSlot.ItemID;
	SlotButton->OnClicked.AddLambda([this, CapturedItemID]()
	{
		UE_LOG(LogTemp, Log, TEXT("InventoryWidget: Item slot clicked for %s"), *CapturedItemID.ToString());
		SelectItem(CapturedItemID);
	});
	
	// Update border style when selection changes (will be refreshed in RefreshInventory)

	// Calculate grid position (6 columns, 3 rows)
	int32 Row = GridIndex / GridColumns;
	int32 Column = GridIndex % GridColumns;

	// Add border (which contains button) to grid
	InventoryGrid->AddChildToUniformGrid(SlotBorder, Row, Column);
}

void UInventoryWidget::ShowInventory()
{
	SetVisibility(ESlateVisibility::Visible);
	RefreshInventory();
	// Note: Input mode is handled by Character::ToggleInventory() to ensure PlayerController is available
}

void UInventoryWidget::HideInventory()
{
	SetVisibility(ESlateVisibility::Collapsed);
	// Note: Input mode is handled by Character::ToggleInventory() to ensure PlayerController is available
}

void UInventoryWidget::ToggleInventory()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		HideInventory();
	}
	else
	{
		ShowInventory();
	}
}

void UInventoryWidget::OnInventoryChanged()
{
	// Refresh display when inventory changes
	RefreshInventory();
}


void UInventoryWidget::SelectItem(FName ItemID)
{
	UE_LOG(LogTemp, Log, TEXT("InventoryWidget: SelectItem called with ItemID: %s"), *ItemID.ToString());
	SelectedItemID = ItemID;
	UpdateDetailsPanel(ItemID);
	RefreshInventory(); // Refresh to update selection highlight
}


void UInventoryWidget::UpdateDetailsPanel(FName ItemID)
{
	if (ItemID == NAME_None)
	{
		// Clear details panel
		if (ItemDetailImage)
		{
			ItemDetailImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (ItemDetailName)
		{
			ItemDetailName->SetText(FText::GetEmpty());
		}
		if (ItemDetailDescription)
		{
			ItemDetailDescription->SetText(FText::GetEmpty());
		}
		return;
	}

	if (!InventoryComponent)
	{
		return;
	}

	// Get item data
	FItemData ItemData;
	if (!InventoryComponent->GetItemData(ItemID, ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryWidget: Could not get item data for %s"), *ItemID.ToString());
		return;
	}

	// Update image
	if (ItemDetailImage)
	{
		if (!ItemData.Icon.IsNull())
		{
			UTexture2D* IconTexture = ItemData.Icon.LoadSynchronous();
			if (IconTexture)
			{
				ItemDetailImage->SetBrushFromTexture(IconTexture);
				ItemDetailImage->SetDesiredSizeOverride(FVector2D(200.0f, 200.0f)); // Smaller size to leave room for description
				ItemDetailImage->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				ItemDetailImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			ItemDetailImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// Update name
	if (ItemDetailName)
	{
		ItemDetailName->SetText(ItemData.DisplayName);
	}

	// Update description
	if (ItemDetailDescription)
	{
		ItemDetailDescription->SetText(ItemData.Description);
	}
}



