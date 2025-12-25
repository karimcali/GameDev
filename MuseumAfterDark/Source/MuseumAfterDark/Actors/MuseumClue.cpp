#include "MuseumAfterDark/Actors/MuseumClue.h"
#include "MuseumAfterDarkCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"

AMuseumClue::AMuseumClue()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create clue mesh component
	ClueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClueMesh"));
	ClueMesh->SetupAttachment(RootComponent);

	// Create collision sphere component
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(100.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Set default clue text
	ClueText = FText::FromString(TEXT("This is a clue. Read it carefully for hints."));
}

void AMuseumClue::BeginPlay()
{
	Super::BeginPlay();
}

void AMuseumClue::Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	ShowClueText();
}

FText AMuseumClue::GetPromptText_Implementation() const
{
	return FText::FromString(TEXT("Press E to Read Clue"));
}

bool AMuseumClue::CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const
{
	// Clues can always be read
	return true;
}

void AMuseumClue::ShowClueText() const
{
	// Display clue text on screen
	if (GEngine)
	{
		FString ClueString = ClueText.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, ClueString);
	}

	// Also log to console
	UE_LOG(LogTemp, Log, TEXT("Clue read: %s"), *ClueText.ToString());
}

