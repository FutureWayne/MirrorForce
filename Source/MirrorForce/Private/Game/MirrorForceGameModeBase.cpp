// Copyright Mirror Force


#include "Game/MirrorForceGameModeBase.h"

#include "EngineUtils.h"

void AMirrorForceGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AMirrorForceLaneController> It(GetWorld()); It; ++It)
	{
		LaneController = *It;
		break;
	}

	check(LaneController != nullptr);
}
