// Copyright Epic Games, Inc. All Rights Reserved.

#include "MirrorForceGameMode.h"
#include "Player/MirrorForcePlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMirrorForceGameMode::AMirrorForceGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMirrorForcePlayerController::StaticClass();
	
}