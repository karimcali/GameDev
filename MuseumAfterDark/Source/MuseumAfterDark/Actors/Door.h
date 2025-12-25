#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuseumAfterDark/Core/Interfaces/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Door.generated.h"

UCLASS()
class MUSEUMAFTERDARK_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	virtual void BeginPlay() override;

	/** Static mesh component for the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	/** Collision component for the door */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> DoorCollision;

	/** Items required to unlock this door */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	TArray<FName> RequiredItems;

	/** Whether the door is currently locked */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsLocked;

	/** Angle to rotate door when opened (in degrees) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle;

	/** Speed of door opening animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenSpeed;

public:
	// IInteractable interface implementation
	virtual void Interact_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) override;
	virtual FText GetPromptText_Implementation() const override;
	virtual bool CanInteract_Implementation(AMuseumAfterDarkCharacter* PlayerCharacter) const override;

private:
	/** Check if player has all required items */
	bool HasAllRequiredItems(AMuseumAfterDarkCharacter* PlayerCharacter) const;

	/** Unlock and open the door */
	void UnlockDoor();
};

