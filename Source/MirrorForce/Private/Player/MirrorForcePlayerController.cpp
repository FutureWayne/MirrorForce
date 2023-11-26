// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/MirrorForcePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/MirrorAbilitySystemComponent.h"
#include "Input/MirrorEnhancedInputComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMirrorForcePlayerController::AMirrorForcePlayerController()
{
}

void AMirrorForcePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AMirrorForcePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	
	// Set up action bindings
	if (UMirrorEnhancedInputComponent* MirrorEnhancedInputComponent = Cast<UMirrorEnhancedInputComponent>(InputComponent)) {

		// Moving
		MirrorEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMirrorForcePlayerController::Move);

		// Ability Input Actions
		MirrorEnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMirrorForcePlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		// add movement
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMirrorForcePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AMirrorForcePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetMirrorAbilitySystemComponent() != nullptr)
	{
		GetMirrorAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	}
}

void AMirrorForcePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetMirrorAbilitySystemComponent() != nullptr)
	{
		GetMirrorAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
}

UMirrorAbilitySystemComponent* AMirrorForcePlayerController::GetMirrorAbilitySystemComponent()
{
	if (MirrorAbilitySystemComponent == nullptr)
	{
		MirrorAbilitySystemComponent = Cast<UMirrorAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return MirrorAbilitySystemComponent;
}
