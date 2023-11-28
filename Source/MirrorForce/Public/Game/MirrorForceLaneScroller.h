// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MirrorForceLaneScroller.generated.h"

UCLASS()
class MIRRORFORCE_API AMirrorForceLaneScroller : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMirrorForceLaneScroller();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Lane Scroller")
	void ChangeToNextScrollingLane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	float LaneScrollSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* LaneActor1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
	AActor* LaneActor2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lane Scroller")
  	AActor* LaneActor3 = nullptr;

private:
	UPROPERTY()
	TArray<AActor*> LaneActors = {};
	
	int CurrentLaneIndex = 0;
};
