#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MuseumAfterDark/Core/Structs/InventorySlot.h"
#include "MuseumAfterDark/Core/Structs/PuzzleState.h"
#include "MuseumSaveGame.generated.h"

/**
 * Save game class for museum game
 */
UCLASS()
class MUSEUMAFTERDARK_API UMuseumSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMuseumSaveGame();

	/** Inventory slots */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	TArray<FInventorySlot> InventorySlots;

	/** Puzzle states */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	TArray<FPuzzleState> PuzzleStates;

	/** Save slot name */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	FString SaveSlotName;

	/** User index */
	UPROPERTY(VisibleAnywhere, Category = "Save")
	int32 UserIndex;
};

