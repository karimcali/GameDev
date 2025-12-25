#pragma once

#include "CoreMinimal.h"
#include "PuzzleState.generated.h"

USTRUCT(BlueprintType)
struct MUSEUMAFTERDARK_API FPuzzleState
{
	GENERATED_BODY()

	FPuzzleState()
		: PuzzleID(NAME_None)
		, bIsComplete(false)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
	FName PuzzleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
	bool bIsComplete;
};

