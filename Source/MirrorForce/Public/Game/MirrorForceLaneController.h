// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MirrorForceLaneController.generated.h"

class ATriggerBox;

USTRUCT(BlueprintType)
struct FLaneInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* LaneActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* AnchorPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	ATriggerBox* EndingTriggerBox = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	ACameraActor* Camera = nullptr;
};

USTRUCT(BlueprintType)
struct FLaneSFXInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane SFX")
	TObjectPtr<USoundBase> ThemeMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane SFX")
	TObjectPtr<USoundBase> VictoryMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane SFX")
	TObjectPtr<USoundBase> LoseMusic;

	UAudioComponent* themeAudioComponent;
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
	const FLaneSFXInfo GetLaneSFXInfo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	float LaneScrollSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	TArray<FLaneInfo> Lanes = {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane SFX")
	TArray<FLaneSFXInfo> LaneSFXs = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	TObjectPtr<AActor> BossActor = nullptr;

private:
	UFUNCTION()
	void OnTriggerBoxOverlap(AActor* OverlappedActor, AActor* OtherActor);
	
	int CurrentLaneIndex = 0;

	TArray<FVector> AnchorPointLocations = {};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> SwitchLaneSFX;

	UAudioComponent* CurrentAudioComponent;
};
