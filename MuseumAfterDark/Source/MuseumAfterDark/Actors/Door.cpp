#include "MuseumAfterDark/Actors/Door.h"
#include "MuseumAfterDarkCharacter.h"
#include "MuseumAfterDark/Core/Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create door mesh component
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(RootComponent);

	// Create collision component
	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
	DoorCollision->SetupAttachment(RootComponent);
	DoorCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorCollision->SetCollisionResponseToAllChannels(ECR_Block);

	// Set default values
	bIsLocked = true;
	OpenAngle = 90.0f;
	OpenSpeed = 2.0f;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// Check if door is already unlocked
	if (!bIsLocked)
	{
		return;
	}

	// Check if player has all required items
	if (HasAllRequiredItems(PlayerCharacter))
	{
		UnlockDoor();
		UE_LOG(LogTemp, Log, TEXT("Door unlocked!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Door is locked. You need the required items."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Door is locked. You need the required items."));
		}
	}
}

FText ADoor::GetPromptText_Implementation() const
{
	if (bIsLocked)
	{
		return FText::FromString(TEXT("Press E (Locked)"));
	}
	else
	{
		return FText::FromString(TEXT("Press E to Open"));
	}
}

bool ADoor::CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const
{
	// Doors can always be interacted with
	return true;
}

bool ADoor::HasAllRequiredItems(AMuseumAfterDarkCharacter* PlayerCharacter) const
{
	if (!PlayerCharacter)
	{
		return false;
	}

	UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return false;
	}

	// Check each required item (simplified - max 3 items)
	for (int32 i = 0; i < RequiredItems.Num() && i < 3; ++i)
	{
		if (!InventoryComponent->HasItem(RequiredItems[i], 1))
		{
			return false;
		}
	}

	return true;
}

void ADoor::UnlockDoor()
{
	bIsLocked = false;

	// Disable collision
	if (DoorCollision)
	{
		DoorCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Rotate door open
	if (DoorMesh)
	{
		FRotator NewRotation = DoorMesh->GetRelativeRotation();
		NewRotation.Yaw += OpenAngle;
		DoorMesh->SetRelativeRotation(NewRotation);
	}
}

