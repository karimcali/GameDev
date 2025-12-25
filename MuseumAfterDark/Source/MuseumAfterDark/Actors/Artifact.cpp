#include "MuseumAfterDark/Actors/Artifact.h"
#include "MuseumAfterDarkCharacter.h"
#include "MuseumAfterDark/Core/Components/InventoryComponent.h"
#include "MuseumAfterDark/Core/Structs/ItemData.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"

AArtifact::AArtifact()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create artifact mesh component
	ArtifactMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArtifactMesh"));
	ArtifactMesh->SetupAttachment(RootComponent);

	// Create collision sphere component
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(100.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// Set default ItemID
	ItemID = NAME_None;
}

void AArtifact::BeginPlay()
{
	Super::BeginPlay();
}

void AArtifact::Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Artifact: Interact called with NULL PlayerCharacter"));
		return;
	}

	// Get inventory component
	UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Artifact: PlayerCharacter has no InventoryComponent!"));
		return;
	}

	// Validate ItemID
	if (ItemID == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("Artifact: ItemID is not set! Cannot add item to inventory."));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Error: Artifact has no ItemID set!"));
		}
		return;
	}

	// Add item to inventory
	bool bAdded = InventoryComponent->AddItem(ItemID, 1);
	if (bAdded)
	{
		UE_LOG(LogTemp, Log, TEXT("Artifact: Successfully added %s to inventory"), *ItemID.ToString());
		
		// Get item display name for feedback
		FText DisplayName = GetItemDisplayName();
		FString ItemName = DisplayName.IsEmpty() ? ItemID.ToString() : DisplayName.ToString();
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%s added to inventory"), *ItemName));
		}

		// Destroy this artifact
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Artifact: Failed to add %s to inventory"), *ItemID.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Failed to add item to inventory"));
		}
	}
}

FText AArtifact::GetPromptText_Implementation() const
{
	FText ItemName = GetItemDisplayName();
	if (!ItemName.IsEmpty())
	{
		return FText::Format(FText::FromString(TEXT("Press E to Pick Up {0}")), ItemName);
	}
	
	// Fallback if ItemID not found in data table
	if (ItemID != NAME_None)
	{
		return FText::Format(FText::FromString(TEXT("Press E to Pick Up {0}")), FText::FromName(ItemID));
	}
	
	return FText::FromString(TEXT("Press E to Pick Up"));
}

bool AArtifact::CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const
{
	// Artifacts can always be picked up
	return true;
}

FText AArtifact::GetItemDisplayName() const
{
	if (ItemID == NAME_None)
	{
		return FText::GetEmpty();
	}

	// Try to get item data from the world's first player character
	// This is a workaround - ideally we'd cache the data table reference
	if (UWorld* World = GetWorld())
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			if (AMuseumAfterDarkCharacter* PlayerCharacter = Cast<AMuseumAfterDarkCharacter>(PC->GetPawn()))
			{
				if (UInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent())
				{
					FItemData ItemData;
					if (InventoryComponent->GetItemData(ItemID, ItemData))
					{
						return ItemData.DisplayName;
					}
				}
			}
		}
	}
	
	return FText::GetEmpty();
}

