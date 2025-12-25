#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumAfterDark/Core/Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Artifact.generated.h"

UCLASS()
class MUSEUMAFTERDARK_API AArtifact : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AArtifact();

protected:
	virtual void BeginPlay() override;

	/** Static mesh component for the artifact */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ArtifactMesh;

	/** Collision component for the artifact */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	/** Item ID for this artifact (must match entry in DT_Items) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifact")
	FName ItemID;

public:
	// IInteractable interface implementation
	virtual void Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) override;
	virtual FText GetPromptText_Implementation() const override;
	virtual bool CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const override;

private:
	/** Get display name from item data */
	FText GetItemDisplayName() const;
};

