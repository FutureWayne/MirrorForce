// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MirrorForceLaneController.generated.h"

USTRUCT(BlueprintType)
struct FLaneInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* LaneActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* Waypoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	ACameraActor* Camera = nullptr;
};

UCLASS()
class MIRRORFORCE_API AMirrorForceLaneController : public AActor
{
	GENERATED_BODY()
	
public:	
	AMirrorForceLaneController();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Lane Scroller")
	void ChangeToNextScrollingLane();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	float LaneScrollSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	TArray<FLaneInfo> Lanes = {};

private:
	int CurrentLaneIndex = 0;

	TArray<FVector> AnchorPointLocations = {};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SpaceMusic;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> AquaticMusic;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MoonMusic;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SwitchLaneSFX;

	UAudioComponent* CurrentAudioComponent;
	UAudioComponent* SpaceAudioComponent;
	UAudioComponent* AquaticAudioComponent;
	UAudioComponent* MoonAudioComponent;
};
