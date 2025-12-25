// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MuseumAfterDarkCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AMuseumAfterDarkCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InteractAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InventoryAction;

	/** Default Input Mapping Context */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	
public:
	AMuseumAfterDarkCharacter();

protected:

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Get Inventory Component */
	class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	/** Inventory Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInventoryComponent* InventoryComponent;

	/** Interaction distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionDistance = 200.0f;

	/** Current interactable object */
	TScriptInterface<class IInteractable> CurrentInteractable;

	/** Interaction prompt widget class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> InteractionPromptWidgetClass;

	/** Interaction prompt widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> InteractionPromptWidget;

	/** Inventory widget class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	/** Inventory widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> InventoryWidget;

	/** HUD widget class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** HUD widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> HUDWidget;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Handle interact input */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact();

	/** Handle inventory toggle input */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ToggleInventory();

	/** Perform interaction trace */
	void PerformInteractionTrace();

	/** Update interaction prompt UI */
	void UpdateInteractionPrompt();

	/** Called when inventory changes - shows pickup notification */
	UFUNCTION()
	void OnInventoryChanged();

	/** Called when timer reaches zero */
	UFUNCTION()
	void OnTimeUp();

private:
	/** Whether inventory is currently open (used to disable mouse look) */
	bool bInventoryOpen;
};

