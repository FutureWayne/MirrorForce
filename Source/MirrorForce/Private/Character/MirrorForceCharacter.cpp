// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MirrorForceCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "Player/MirrorForcePlayerState.h"
#include "UI/HUD/MirrorForceHUD.h"

AMirrorForceCharacter::AMirrorForceCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	PrimaryActorTick.bCanEverTick = true;
}


void AMirrorForceCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMirrorForceCharacter::InitAbilityActorInfo()
{
	AMirrorForcePlayerState* MirrorForcePlayerState = GetPlayerState<AMirrorForcePlayerState>();
	check(MirrorForcePlayerState);
	AbilitySystemComponent = MirrorForcePlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(MirrorForcePlayerState, this);
	AttributeSet = MirrorForcePlayerState->GetAttributeSet();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()) )
	{
		if ( AMirrorForceHUD* MirrorForceHUD = Cast<AMirrorForceHUD>(PlayerController->GetHUD()) )
		{
			MirrorForceHUD->InitOverlay(PlayerController, MirrorForcePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AMirrorForceCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddCharacterAbilities();
}
