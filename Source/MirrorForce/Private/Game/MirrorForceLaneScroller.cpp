// Copyright Mirror Force


#include "Game/MirrorForceLaneScroller.h"

AMirrorForceLaneScroller::AMirrorForceLaneScroller()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMirrorForceLaneScroller::BeginPlay()
{
	Super::BeginPlay();
	// populate lane actors array
	LaneActors.Add(LaneActor1);
	LaneActors.Add(LaneActor2);
	LaneActors.Add(LaneActor3);
}

void AMirrorForceLaneScroller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (LaneActors.IsValidIndex(CurrentLaneIndex))
	{
		AActor* CurrentLane = LaneActors[CurrentLaneIndex];
		if (CurrentLane)
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

void AMirrorForceLaneScroller::ChangeToNextScrollingLane()
{
	CurrentLaneIndex = (CurrentLaneIndex + 1) % LaneActors.Num();
}

