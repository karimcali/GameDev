#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumAfterDark/Core/Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MuseumClue.generated.h"

UCLASS()
class MUSEUMAFTERDARK_API AMuseumClue : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AMuseumClue();

protected:
	virtual void BeginPlay() override;

	/** Static mesh component for the clue */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ClueMesh;

	/** Collision component for the clue */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	/** Text content of the clue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clue")
	FText ClueText;

public:
	// IInteractable interface implementation
	virtual void Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) override;
	virtual FText GetPromptText_Implementation() const override;
	virtual bool CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const override;

private:
	/** Display the clue text to the player */
	void ShowClueText() const;
};

