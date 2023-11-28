// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/MirrorForceBulletPool.h"
#include "MirrorForceBulletPattern.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRRORFORCE_API UMirrorForceBulletPattern : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMirrorForceBulletPattern();

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnCirclePattern(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleBetweenBullets, float BulletSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnMultipleCircles(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleBetweenBullets, float BulletSpeed, int NumSpirals, float DelayBetweenSpirals);
	
	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnSpiralPattern(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleIncrement, float BulletSpeed, float DelayBetweenBullets);

	UFUNCTION()
	void SpawnBullet(UMirrorForceBulletPool* BulletPool, float Angle, float BulletSpeed);
};
