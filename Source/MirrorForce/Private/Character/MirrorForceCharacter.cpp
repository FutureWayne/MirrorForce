// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MirrorForceCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/MirrorAttributeSet.h"
#include "Player/MirrorForcePlayerState.h"
#include "UI/HUD/MirrorForceHUD.h"
#include <Kismet/GameplayStatics.h>
#include <Game/MirrorForceLaneController.h>
#include <Game/MirrorForceGameModeBase.h>

#include "NiagaraComponent.h"

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
	GetCharacterMovement()->GravityScale = 0.0f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	
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

	// Bind on mana change delegate
	const UMirrorAttributeSet* MirrorAttributeSet = Cast<UMirrorAttributeSet>(MirrorForcePlayerState->GetAttributeSet());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MirrorAttributeSet->GetHealthAttribute()).AddUObject(this, &AMirrorForceCharacter::OnHealthChange);

	// Get LaneController
	AMirrorForceLaneController* LaneController = Cast<AMirrorForceLaneController>(UGameplayStatics::GetActorOfClass(GetWorld(), AMirrorForceLaneController::StaticClass()));
	
	// Init UI Overlay
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()) )
	{
		if ( AMirrorForceHUD* MirrorForceHUD = Cast<AMirrorForceHUD>(PlayerController->GetHUD()) )
		{
			MirrorForceHUD->InitOverlay(PlayerController, MirrorForcePlayerState, AbilitySystemComponent, AttributeSet, LaneController);
		}
	}
}

void AMirrorForceCharacter::OnPlayerDead()
{
	if (bIsDead)
	{
		return;
	}
	
	if (const AMirrorForceGameModeBase* GameMode = Cast<AMirrorForceGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		AMirrorForceLaneController* LaneController = GameMode->LaneController;
		LaneController->OnPlayerDead();
		
		LaneController->StopThemeMusic();
		const TObjectPtr<USoundBase> LoseSFX = LaneController->GetLaneSFXInfo().LoseMusic;
		UGameplayStatics::SpawnSoundAtLocation(this, LoseSFX, GetActorLocation());
	}

	if (UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		DeathEffect,
		GetActorLocation(),
		GetActorRotation()
	))
	{
		NiagaraComponent->SetAutoDestroy(true);
	}

	// Disable input
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlayerController->DisableInput(PlayerController);
	}

	// Hide skeletal mesh
	GetMesh()->SetVisibility(false, true);

	// Disable capsule collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsDead = true;
}

void AMirrorForceCharacter::OnHealthChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue <= 0.0f)
	{
		OnPlayerDead();
	}
}

void AMirrorForceCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();

	AddCharacterAbilities();
}