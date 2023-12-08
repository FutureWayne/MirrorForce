// Copyright Mirror Force


#include "Game/MirrorForceLaneController.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerBox.h"

AMirrorForceLaneController::AMirrorForceLaneController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMirrorForceLaneController::BeginPlay()
{
	Super::BeginPlay();

	for (const auto LaneInfo : Lanes)
	{
		check(LaneInfo.Camera != nullptr);
		check(LaneInfo.LaneActor != nullptr);
		check(LaneInfo.AnchorPoint != nullptr);

		// Setting up camera location and anchor point locations for each lane
		const UStaticMeshComponent* MeshComponent = LaneInfo.AnchorPoint->FindComponentByClass<UStaticMeshComponent>();
		FBox BoundingBox = MeshComponent->CalcBounds(MeshComponent->GetComponentTransform()).GetBox();
		FVector CenterLocation = BoundingBox.GetCenter();
		CenterLocation.X -= 1000.f;
		CenterLocation.Z += 2000.f;
		AnchorPointLocations.Add(CenterLocation);
		
		LaneInfo.Camera->SetActorLocation(CenterLocation);

		LaneInfo.EndingTriggerBox->OnActorBeginOverlap.AddDynamic(this, &AMirrorForceLaneController::OnTriggerBoxOverlap);
	}

	// Use the first camera as the starting camera
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetViewTarget(Lanes[0].Camera);
	}

	//Set up theme music audio component
	for (int i = 0; i < LaneSFXs.Num(); i++)
	{
		check(LaneSFXs.IsValidIndex(i))
		LaneSFXs[i].ThemeAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, LaneSFXs[i].ThemeMusic, GetActorLocation());
		LaneSFXs[i].ThemeAudioComponent->SetPaused(true);
		if (!CurrentAudioComponent)
		{
			CurrentAudioComponent = LaneSFXs[i].ThemeAudioComponent;
			CurrentAudioComponent->SetPaused(false);
			CurrentAudioComponent->bIsUISound = true;
		}
	}

	// Calculate lane length from anchor point to ending trigger box
	check(Lanes.IsValidIndex(0))
	const FVector AnchorPointLocation = AnchorPointLocations[0];
	const FVector EndingTriggerBoxLocation = Lanes[0].EndingTriggerBox->GetActorLocation();
	LaneLength = EndingTriggerBoxLocation.X - AnchorPointLocation.X;

	// Init lane progress
	for	(int i = 0; i < Lanes.Num(); i++)
	{
		LaneProgress.Add(0.0f);
	}
}

void AMirrorForceLaneController::OnTriggerBoxOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (LaneSFXs.IsValidIndex(CurrentLaneIndex))
		{
			CurrentAudioComponent->SetSound(LaneSFXs[CurrentLaneIndex].VictoryMusic);
			bShouldScroll = false;

			//TODO: Winning UI
			OnGameWin.Broadcast();

			// pause the game
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void AMirrorForceLaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bShouldScroll)
	{
		return;
	}
	
	if (Lanes.IsValidIndex(CurrentLaneIndex))
	{
		if (AActor* CurrentLane = Lanes[CurrentLaneIndex].LaneActor)
		{
			FVector CurrentLaneLocation = CurrentLane->GetActorLocation();
			CurrentLaneLocation.X += LaneScrollSpeed * DeltaTime;
			CurrentLane->SetActorLocation(CurrentLaneLocation);

			// Update lane progress
			LaneProgress[CurrentLaneIndex] = (LaneLength - CurrentLaneLocation.X + AnchorPointLocations[CurrentLaneIndex].X) / LaneLength;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentLane is NULL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentLaneIndex is out of bounds"));
	}
}

void AMirrorForceLaneController::ChangeToNextScrollingLane()
{
	const FVector LastAnchorPointLocation = AnchorPointLocations[CurrentLaneIndex];

	// Change to next lane index
	CurrentLaneIndex = (CurrentLaneIndex + 1) % Lanes.Num();

	// Use the next camera as the new view target
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->SetViewTarget(Lanes[CurrentLaneIndex].Camera);

	const FVector NewAnchorPointLocation = AnchorPointLocations[CurrentLaneIndex];
	const FVector DeltaLocation = NewAnchorPointLocation - LastAnchorPointLocation;

	// Move player by delta location
	if (AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		Player->SetActorLocation(Player->GetActorLocation() + DeltaLocation);
		if (UNiagaraComponent* NiagaraComponent = Player->FindComponentByClass<UNiagaraComponent>())
		{
			// Activate the Niagara system
			NiagaraComponent->Activate();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NiagaraComponent not found"));
		}
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is NULL"));
	}

	// Move Boss Actor by delta location
	if (BossActor)
	{
		BossActor->SetActorLocation(BossActor->GetActorLocation() + DeltaLocation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BossActor is NULL"));
	}
	
	//Switch lanes theme music
	UGameplayStatics::PlaySoundAtLocation(this, SwitchLaneSFX, GetActorLocation());
	check(LaneSFXs.IsValidIndex(CurrentLaneIndex));
	CurrentAudioComponent->SetPaused(true);
	LaneSFXs[CurrentLaneIndex].ThemeAudioComponent->SetPaused(false);
	CurrentAudioComponent = LaneSFXs[CurrentLaneIndex].ThemeAudioComponent;
	CurrentAudioComponent->bIsUISound = true;
}

float AMirrorForceLaneController::GetCurrentLaneProgress() const
{
	return LaneProgress[CurrentLaneIndex];
}

FLaneSFXInfo AMirrorForceLaneController::GetLaneSFXInfo()
{
	return LaneSFXs[CurrentLaneIndex];
}

void AMirrorForceLaneController::StopThemeMusic()
{
	CurrentAudioComponent->SetPaused(true);
}

void AMirrorForceLaneController::OnPlayerDead()
{
	bShouldScroll = false;

	OnGameLose.Broadcast();
}

