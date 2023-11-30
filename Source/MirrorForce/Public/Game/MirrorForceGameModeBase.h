// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "MirrorForceLaneController.h"
#include "GameFramework/GameModeBase.h"
#include "MirrorForceGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API AMirrorForceGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Reference to the Lane Controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	AMirrorForceLaneController* LaneController;
};
