#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MuseumAfterDark/Core/Managers/PuzzleManager.h"
#include "MuseumGameState.generated.h"

/**
 * Game state for the museum game
 */
UCLASS()
class MUSEUMAFTERDARK_API AMuseumGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMuseumGameState();

	/** Get the puzzle manager */
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	UPuzzleManager* GetPuzzleManager() const { return PuzzleManager; }

protected:
	/** Puzzle manager instance */
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	TObjectPtr<UPuzzleManager> PuzzleManager;
};

