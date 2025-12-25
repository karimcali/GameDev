#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Core/Components/InventoryComponent.h"
#include "../Core/Structs/ItemData.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UTextBlock;
class UImage;
class UButton;
class UBorder;

/**
 * Widget that displays the player's inventory in a 3x6 grid (18 items)
 */
UCLASS(BlueprintType, Blueprintable)
class MUSEUMAFTERDARK_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Initialize the widget with inventory component */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory(UInventoryComponent* InInventoryComponent);

	/** Refresh the inventory display */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RefreshInventory();

	/** Show the inventory widget */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ShowInventory();

	/** Hide the inventory widget */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideInventory();

	/** Toggle inventory visibility */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	/** Select an item to show details */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SelectItem(FName ItemID);

protected:
	virtual void NativeConstruct() override;

	/** Called when inventory changes */
	UFUNCTION()
	void OnInventoryChanged();

	/** Grid panel for inventory items (3 rows, 6 columns = 18 items total) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;

	/** Large image for selected item */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemDetailImage;

	/** Text block for selected item name */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDetailName;

	/** Text block for selected item description */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDetailDescription;

	/** Reference to the inventory component */
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

private:
	/** Currently selected item ID */
	FName SelectedItemID;

	/** Number of columns in grid */
	static constexpr int32 GridColumns = 6;

	/** Number of rows in grid */
	static constexpr int32 GridRows = 3;

	/** Max items displayed (3 rows x 6 columns = 18) */
	static constexpr int32 MaxItemsDisplayed = 18;

	/** Create a widget for a single inventory slot */
	void CreateSlotWidget(const FInventorySlot& InvSlot, int32 SlotIndex, int32 GridIndex);

	/** Update the details panel with item information */
	void UpdateDetailsPanel(FName ItemID);
};
