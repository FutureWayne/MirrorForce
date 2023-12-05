// Copyright Mirror Force


#include "Game/MirrorForceLaneController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
		check(LaneInfo.Waypoint != nullptr);

		// Setting up camera location and anchor point locations for each lane
		const UStaticMeshComponent* MeshComponent = LaneInfo.Waypoint->FindComponentByClass<UStaticMeshComponent>();
		FBox BoundingBox = MeshComponent->CalcBounds(MeshComponent->GetComponentTransform()).GetBox();
		FVector CenterLocation = BoundingBox.GetCenter();
		CenterLocation.X -= 1000.f;
		CenterLocation.Z += 2000.f;
		AnchorPointLocations.Add(CenterLocation);
		
		LaneInfo.Camera->SetActorLocation(CenterLocation);
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

	SpaceAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, SpaceMusic, GetActorLocation());
	AquaticAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, AquaticMusic, GetActorLocation());
	MoonAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, MoonMusic, GetActorLocation());

	SpaceAudioComponent->SetPaused(true);
	AquaticAudioComponent->SetPaused(true);
	MoonAudioComponent->SetPaused(true);

	CurrentAudioComponent = AquaticAudioComponent;
	CurrentAudioComponent->SetPaused(false);
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

	// TODO: Move Boss by delta location

	//Switch lanes theme music
	UGameplayStatics::PlaySoundAtLocation(this, SwitchLaneSFX, GetActorLocation());
	switch (CurrentLaneIndex)
	{
		case 0: //Aquatic
			CurrentAudioComponent->SetPaused(true);
			AquaticAudioComponent->SetPaused(false);
			CurrentAudioComponent = AquaticAudioComponent;
			break;
		case 1: //Moon
			CurrentAudioComponent->SetPaused(true);
			MoonAudioComponent->SetPaused(false);
			CurrentAudioComponent = MoonAudioComponent;
			break;
		case 2: //Space
			CurrentAudioComponent->SetPaused(true); 
			SpaceAudioComponent->SetPaused(false);
			CurrentAudioComponent = SpaceAudioComponent;
			break;
	}
}

