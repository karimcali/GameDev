#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemTag : uint8
{
	None            UMETA(DisplayName = "None"),
	KeyItem         UMETA(DisplayName = "Key Item"),
	Lore            UMETA(DisplayName = "Lore"),
	Tool            UMETA(DisplayName = "Tool"),
	Fragment        UMETA(DisplayName = "Fragment"),
	Egyptian        UMETA(DisplayName = "Egyptian"),
	Medieval        UMETA(DisplayName = "Medieval"),
	Victorian       UMETA(DisplayName = "Victorian")
};

USTRUCT(BlueprintType)
struct MUSEUMAFTERDARK_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

	FItemData()
		: ItemID(NAME_None)
		, DisplayName(FText::FromString(TEXT("Unknown Item")))
		, Era(TEXT("Unknown Era"))
		, Origin(TEXT("Unknown Origin"))
		, Description(FText::FromString(TEXT("No description available.")))
		, bStackable(false)
		, MaxStack(1)
		, CombineGroupID(NAME_None)
		, ItemTags()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UStaticMesh> InspectMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Era;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Origin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 MaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName CombineGroupID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<EItemTag> ItemTags;
};

