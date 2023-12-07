// Copyright Mirror Force


#include "Game/MirrorForceLaneController.h"

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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is NULL"));
	}

	//Set up theme music audio component
	for (int i = 0; i < LaneSFXs.Num(); i++)
	{
		if (LaneSFXs.IsValidIndex(i))
		{
			LaneSFXs[i].themeAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, LaneSFXs[i].ThemeMusic, GetActorLocation());
			LaneSFXs[i].themeAudioComponent->SetPaused(true);
			if (!CurrentAudioComponent)
			{
				CurrentAudioComponent = LaneSFXs[i].themeAudioComponent;
				CurrentAudioComponent->SetPaused(false);
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No SFX assign in bp!"));
		}
	}
}

void AMirrorForceLaneController::OnTriggerBoxOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		// TODO: Winning Condition
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("WIN!"));
		if (LaneSFXs.IsValidIndex(CurrentLaneIndex))
		{
			CurrentAudioComponent->SetPaused(true);
			UGameplayStatics::SpawnSoundAtLocation(this, LaneSFXs[CurrentLaneIndex].VictoryMusic, GetActorLocation());
		}
	}
}

void AMirrorForceLaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Lanes.IsValidIndex(CurrentLaneIndex))
	{
		if (AActor* CurrentLane = Lanes[CurrentLaneIndex].LaneActor)
		{
			FVector CurrentLaneLocation = CurrentLane->GetActorLocation();
			CurrentLaneLocation.X += LaneScrollSpeed * DeltaTime;
			CurrentLane->SetActorLocation(CurrentLaneLocation);
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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetViewTarget(Lanes[CurrentLaneIndex].Camera);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is NULL"));
	}

	const FVector NewAnchorPointLocation = AnchorPointLocations[CurrentLaneIndex];
	const FVector DeltaLocation = NewAnchorPointLocation - LastAnchorPointLocation;

	// Move player by delta location
	if (AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		Player->SetActorLocation(Player->GetActorLocation() + DeltaLocation);
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
	if (LaneSFXs.IsValidIndex(CurrentLaneIndex))
	{
		CurrentAudioComponent->SetPaused(true);
		LaneSFXs[CurrentLaneIndex].themeAudioComponent->SetPaused(false);
		CurrentAudioComponent = LaneSFXs[CurrentLaneIndex].themeAudioComponent;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can't find SFX"));
	}
}

FLaneSFXInfo AMirrorForceLaneController::GetLaneSFXInfo()
{
	return LaneSFXs[CurrentLaneIndex];
}

void AMirrorForceLaneController::StopThemeMusic()
{
	CurrentAudioComponent->SetPaused(true);
}

