#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Structs/ItemData.h"
#include "../Structs/InventorySlot.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MUSEUMAFTERDARK_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	/** Add item to inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemID, int32 Quantity = 1);

	/** Remove item from inventory */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Quantity = 1);

	/** Check if player has item */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemID, int32 Quantity = 1) const;

	/** Get quantity of item */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemQuantity(FName ItemID) const;

	/** Check if player has any item with tag */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasTag(EItemTag Tag) const;

	/** Combine fragments */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CombineFragments(FName CombineGroupID);

	/** Get all inventory slots */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventorySlot> GetInventorySlots() const { return InventorySlots; }

	/** Get item data from data table */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool GetItemData(FName ItemID, FItemData& OutItemData) const;

	/** Event fired when inventory changes */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

protected:
	/** Data table containing item definitions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UDataTable> ItemsDataTable;

	/** Maximum number of inventory slots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSlots;

	/** Array of inventory slots */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

	/** Find slot containing item */
	int32 FindItemSlot(FName ItemID) const;

	/** Find empty slot */
	int32 FindEmptySlot() const;
};

