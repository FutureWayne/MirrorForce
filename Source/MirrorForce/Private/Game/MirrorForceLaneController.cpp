// Copyright Mirror Force


#include "Game/MirrorForceLaneController.h"

#include "Camera/CameraActor.h"

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
		CenterLocation.Z += 500.f;
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
}

