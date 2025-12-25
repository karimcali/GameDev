#include "MuseumAfterDark/GameModes/MuseumGameState.h"

AMuseumGameState::AMuseumGameState()
{
	PuzzleManager = CreateDefaultSubobject<UPuzzleManager>(TEXT("PuzzleManager"));
}

