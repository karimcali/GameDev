#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Structs/PuzzleState.h"
#include "PuzzleManager.generated.h"

/**
 * Manages puzzle state and progression
 */
UCLASS(BlueprintType)
class MUSEUMAFTERDARK_API UPuzzleManager : public UObject
{
	GENERATED_BODY()

public:
	UPuzzleManager();

	/**
	 * Mark a puzzle as complete
	 * @param PuzzleID The ID of the puzzle
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void CompletePuzzle(FName PuzzleID);

	/**
	 * Check if a puzzle is complete
	 * @param PuzzleID The ID of the puzzle
	 * @return True if puzzle is complete
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	bool IsPuzzleComplete(FName PuzzleID) const;

	/**
	 * Get all puzzle states
	 * @return Array of puzzle states
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	TArray<FPuzzleState> GetPuzzleStates() const { return PuzzleStates; }

protected:
	/** Array of puzzle states */
	UPROPERTY(BlueprintReadOnly, Category = "Puzzle")
	TArray<FPuzzleState> PuzzleStates;

	/** Find puzzle state by ID */
	int32 FindPuzzleIndex(FName PuzzleID) const;
};

