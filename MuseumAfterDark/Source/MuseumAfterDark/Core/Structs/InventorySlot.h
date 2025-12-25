#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

USTRUCT(BlueprintType)
struct MUSEUMAFTERDARK_API FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot()
		: ItemID(NAME_None)
		, Quantity(0)
	{}

	FInventorySlot(FName InItemID, int32 InQuantity)
		: ItemID(InItemID)
		, Quantity(InQuantity)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	bool IsEmpty() const { return ItemID == NAME_None || Quantity <= 0; }
};

