#include "MuseumAfterDark/Core/Managers/PuzzleManager.h"

UPuzzleManager::UPuzzleManager()
{
}

void UPuzzleManager::CompletePuzzle(FName PuzzleID)
{
	if (PuzzleID == NAME_None)
	{
		return;
	}

	int32 Index = FindPuzzleIndex(PuzzleID);
	if (Index != INDEX_NONE)
	{
		PuzzleStates[Index].bIsComplete = true;
	}
	else
	{
		FPuzzleState NewState;
		NewState.PuzzleID = PuzzleID;
		NewState.bIsComplete = true;
		PuzzleStates.Add(NewState);
	}
}

bool UPuzzleManager::IsPuzzleComplete(FName PuzzleID) const
{
	int32 Index = FindPuzzleIndex(PuzzleID);
	if (Index != INDEX_NONE)
	{
		return PuzzleStates[Index].bIsComplete;
	}

	return false;
}

int32 UPuzzleManager::FindPuzzleIndex(FName PuzzleID) const
{
	for (int32 i = 0; i < PuzzleStates.Num(); ++i)
	{
		if (PuzzleStates[i].PuzzleID == PuzzleID)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

