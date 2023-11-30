// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorPool/MirrorForceActorPool.h"
#include "MirrorForceBulletSpawner.generated.h"


UCLASS()
class MIRRORFORCE_API AMirrorForceBulletSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMirrorForceBulletSpawner();

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnCirclePattern(int NumBullets, float AngleBetweenBullets, float BulletSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnMultipleCircles(int NumBullets, float AngleBetweenBullets, float BulletSpeed, int NumSpirals, float DelayBetweenSpirals);
	
	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnSpiralPattern(int NumBullets, float AngleIncrement, float BulletSpeed, float DelayBetweenBullets);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnBullet(float Angle, float BulletSpeed) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Pattern")
	TObjectPtr<UMirrorForceActorPool> BulletPool;
};
