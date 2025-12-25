// Copyright Epic Games, Inc. All Rights Reserved.

#include "MuseumAfterDarkCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "MuseumAfterDark.h"
#include "MuseumAfterDark/Core/Components/InventoryComponent.h"
#include "MuseumAfterDark/Core/Interfaces/Interactable.h"
#include "MuseumAfterDark/UI/InventoryWidget.h"
#include "MuseumAfterDark/UI/HUDWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMuseumAfterDarkCharacter::AMuseumAfterDarkCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Create inventory component
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	// Initialize inventory state
	bInventoryOpen = false;
}

void AMuseumAfterDarkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMuseumAfterDarkCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMuseumAfterDarkCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMuseumAfterDarkCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMuseumAfterDarkCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMuseumAfterDarkCharacter::LookInput);

		// Interacting
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMuseumAfterDarkCharacter::Interact);
		}

		// Inventory
		if (InventoryAction)
		{
			EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AMuseumAfterDarkCharacter::ToggleInventory);
		}
	}
	else
	{
		UE_LOG(LogMuseumAfterDark, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AMuseumAfterDarkCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AMuseumAfterDarkCharacter::LookInput(const FInputActionValue& Value)
{
	// Don't process look input when inventory is open
	if (bInventoryOpen)
	{
		return;
	}
	
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AMuseumAfterDarkCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMuseumAfterDarkCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AMuseumAfterDarkCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AMuseumAfterDarkCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AMuseumAfterDarkCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}

		// Create interaction prompt widget
		if (InteractionPromptWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creating widget from class: %s"), *InteractionPromptWidgetClass->GetName());
			InteractionPromptWidget = CreateWidget<UUserWidget>(PlayerController, InteractionPromptWidgetClass);
			if (InteractionPromptWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("Widget created successfully: %s"), *InteractionPromptWidget->GetName());
				InteractionPromptWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("Widget added to viewport"));
				// Hide widget initially by calling Blueprint function
				if (UFunction* HidePromptFunc = InteractionPromptWidget->FindFunction("HidePrompt"))
				{
					UE_LOG(LogTemp, Warning, TEXT("HidePrompt function found, calling it"));
					InteractionPromptWidget->ProcessEvent(HidePromptFunc, nullptr);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("HidePrompt function NOT found, using SetVisibility"));
					InteractionPromptWidget->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create widget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InteractionPromptWidgetClass is NULL! Check BP_MuseumPlayer Class Defaults."));
		}

		// Create HUD widget
		if (HUDWidgetClass)
		{
			HUDWidget = CreateWidget<UUserWidget>(PlayerController, HUDWidgetClass);
			if (HUDWidget)
			{
			HUDWidget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("HUD Widget created and added to viewport"));
			
			// Bind to timer time up event (timer starts automatically in NativeConstruct)
			if (UHUDWidget* HUD = Cast<UHUDWidget>(HUDWidget))
			{
				HUD->OnTimeUp.AddDynamic(this, &AMuseumAfterDarkCharacter::OnTimeUp);
			}
		}
		}

		// Bind to inventory changed delegate
		if (InventoryComponent)
		{
			InventoryComponent->OnInventoryChanged.AddDynamic(this, &AMuseumAfterDarkCharacter::OnInventoryChanged);
		}
	}
}

void AMuseumAfterDarkCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PerformInteractionTrace();
	UpdateInteractionPrompt();
}

void AMuseumAfterDarkCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interact() called"));
	
	UObject* InteractableObject = CurrentInteractable.GetObject();
	if (InteractableObject != nullptr && InteractableObject->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact(): Calling Execute_Interact on %s"), *InteractableObject->GetName());
		IInteractable::Execute_Interact(InteractableObject, this);
		UE_LOG(LogTemp, Warning, TEXT("Interact(): Execute_Interact completed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Interact(): CurrentInteractable is NULL or invalid"));
	}
}

void AMuseumAfterDarkCharacter::ToggleInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("Character: ToggleInventory() CALLED!"));
	
	if (!InventoryWidget && InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character: Creating inventory widget. Class: %s"), InventoryWidgetClass ? *InventoryWidgetClass->GetName() : TEXT("NULL"));
		
		// Create inventory widget if it doesn't exist
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("Character: Widget created successfully"));
			InventoryWidget->AddToViewport();
			
			// Initialize with inventory component
			if (UInventoryWidget* InvWidget = Cast<UInventoryWidget>(InventoryWidget))
			{
				InvWidget->InitializeInventory(InventoryComponent);
				UE_LOG(LogTemp, Warning, TEXT("Character: Inventory widget initialized"));
			}
			
			// Hide by default
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character: Failed to create inventory widget!"));
		}
	}

	if (InventoryWidget)
	{
		// Get player controller for input mode
		APlayerController* PC = Cast<APlayerController>(GetController());
		UE_LOG(LogTemp, Warning, TEXT("Character: PlayerController: %s"), PC ? *PC->GetName() : TEXT("NULL"));
		
		// Toggle visibility
		if (UInventoryWidget* InvWidget = Cast<UInventoryWidget>(InventoryWidget))
		{
			bool bIsVisible = InventoryWidget->GetVisibility() == ESlateVisibility::Visible;
			UE_LOG(LogTemp, Warning, TEXT("Character: Inventory currently visible: %d"), bIsVisible);
			
			InvWidget->ToggleInventory();
			
			// Set input mode based on visibility
			if (PC)
			{
				if (!bIsVisible) // Inventory is now showing
				{
					bInventoryOpen = true;
					UE_LOG(LogTemp, Warning, TEXT("Character: SHOWING inventory - setting up input mode"));
					
					// Use GameAndUI input mode - keeps Enhanced Input active so TAB still works
					// Mouse look is disabled via bInventoryOpen flag in LookInput()
					FInputModeGameAndUI InputMode;
					InputMode.SetLockMouseToViewport(false);
					InputMode.SetHideCursorDuringCapture(false);
					
					PC->SetInputMode(InputMode);
					UE_LOG(LogTemp, Warning, TEXT("Character: SetInputMode(GameAndUI) called"));
					
					// Show mouse cursor
					PC->SetShowMouseCursor(true);
					UE_LOG(LogTemp, Warning, TEXT("Character: SetShowMouseCursor(true) called"));
				}
				else // Inventory is now hiding
				{
					bInventoryOpen = false;
					UE_LOG(LogTemp, Warning, TEXT("Character: HIDING inventory - restoring input mode"));
					
					// Hide mouse cursor and restore game input
					FInputModeGameOnly InputMode;
					PC->SetInputMode(InputMode);
					PC->SetShowMouseCursor(false);
					UE_LOG(LogTemp, Warning, TEXT("Character: SetInputMode(GameOnly) and SetShowMouseCursor(false) called"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Character: PlayerController is NULL! Cannot set input mode."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Character: Failed to cast InventoryWidget to UInventoryWidget!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Character: InventoryWidget is NULL!"));
	}
}

void AMuseumAfterDarkCharacter::PerformInteractionTrace()
{
	FVector CameraLocation = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForward = FirstPersonCameraComponent->GetForwardVector();
	FVector TraceEnd = CameraLocation + (CameraForward * InteractionDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	FHitResult HitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && HitResult.GetActor())
	{
		// Check if hit actor implements IInteractable
		if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			TScriptInterface<IInteractable> NewInteractable = HitResult.GetActor();
			
			// Check if we can interact
			if (IInteractable::Execute_CanInteract(NewInteractable.GetObject(), this))
			{
				CurrentInteractable = NewInteractable;
				UE_LOG(LogTemp, Log, TEXT("PerformInteractionTrace: Set CurrentInteractable to %s"), *HitResult.GetActor()->GetName());
			}
			else
			{
				CurrentInteractable = nullptr;
				UE_LOG(LogTemp, Log, TEXT("PerformInteractionTrace: CanInteract returned false, clearing CurrentInteractable"));
			}
		}
		else
		{
			CurrentInteractable = nullptr;
		}
	}
	else
	{
		CurrentInteractable = nullptr;
	}
}

void AMuseumAfterDarkCharacter::OnInventoryChanged()
{
	// Inventory changed - HUD can update if needed
	// (No pickup toast in simplified HUD)
}

void AMuseumAfterDarkCharacter::OnTimeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Time's up! Game Over."));
	
	// Restart the level
	if (UWorld* World = GetWorld())
	{
		FString CurrentLevelName = World->GetMapName();
		CurrentLevelName.RemoveFromStart(World->StreamingLevelsPrefix);
		UGameplayStatics::OpenLevel(World, FName(*CurrentLevelName));
	}
}

void AMuseumAfterDarkCharacter::UpdateInteractionPrompt()
{
	if (!InteractionPromptWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateInteractionPrompt: Widget is NULL!"));
		return;
	}

	// Check if CurrentInteractable is valid
	UObject* InteractableObject = CurrentInteractable.GetObject();
	bool bHasInteractable = (InteractableObject != nullptr && InteractableObject->GetClass()->ImplementsInterface(UInteractable::StaticClass()));
	UE_LOG(LogTemp, Log, TEXT("UpdateInteractionPrompt: CurrentInteractable object = %s, valid = %d"), 
		InteractableObject ? *InteractableObject->GetName() : TEXT("NULL"), bHasInteractable);
	
	if (bHasInteractable)
	{
		// Get prompt text from interactable
		FText PromptText = IInteractable::Execute_GetPromptText(CurrentInteractable.GetObject());
		UE_LOG(LogTemp, Log, TEXT("UpdateInteractionPrompt: Got prompt text: %s"), *PromptText.ToString());
		
		// Call Blueprint function SetPromptText
		if (UFunction* SetPromptTextFunc = InteractionPromptWidget->FindFunction("SetPromptText"))
		{
			UE_LOG(LogTemp, Log, TEXT("SetPromptText function found, calling it"));
			InteractionPromptWidget->ProcessEvent(SetPromptTextFunc, &PromptText);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SetPromptText function NOT found!"));
		}
		
		// Call Blueprint function ShowPrompt
		if (UFunction* ShowPromptFunc = InteractionPromptWidget->FindFunction("ShowPrompt"))
		{
			UE_LOG(LogTemp, Warning, TEXT("ShowPrompt function found, calling it"));
			InteractionPromptWidget->ProcessEvent(ShowPromptFunc, nullptr);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ShowPrompt function NOT found! Using fallback SetVisibility"));
			// Fallback: set visibility directly
			InteractionPromptWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UpdateInteractionPrompt: No interactable, hiding prompt"));
		// Call Blueprint function HidePrompt
		if (UFunction* HidePromptFunc = InteractionPromptWidget->FindFunction("HidePrompt"))
		{
			InteractionPromptWidget->ProcessEvent(HidePromptFunc, nullptr);
		}
		else
		{
			// Fallback: set visibility directly
			InteractionPromptWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
