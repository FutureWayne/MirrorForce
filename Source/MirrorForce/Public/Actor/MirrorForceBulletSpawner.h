// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorPool/MirrorForceActorPool.h"
#include "Actor/MirrorForceProjectile.h"
#include "MirrorForceBulletSpawner.generated.h"


UCLASS()
class MIRRORFORCE_API AMirrorForceBulletSpawner : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMirrorForceBulletSpawner();

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnCirclePattern(int InNumBullets, float AngleBetweenBullets, float InBulletSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnMultipleCircles(int InNumBullets, float AngleBetweenBullets, float InBulletSpeed, int NumSpirals, float DelayBetweenSpirals);
	
	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnSpiralPattern(int InNumBullets, float InAngleIncrement, float InBulletSpeed, float DelayBetweenBullets);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnBullet(float Angle, float InBulletSpeed) const;

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnHoveringPattern(int InNumBullets, float DelayBetweenBullets,float InBulletSpeed, AActor* InPlayer);

	UFUNCTION()
	void SpawnBulletWithTimer();

	UFUNCTION()
	void FireHoveringBullet();

	UFUNCTION()
	void SpawnSingleHoveringBullet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Pattern")
	TObjectPtr<UMirrorForceActorPool> BulletPool;

private:
	int CurrentBullet = 0;
	int NumBullets;
	float AngleIncrement;
	float BulletSpeed;
	
	TArray<AMirrorForceProjectile*> HoveringBullets;
	AActor* Player; 
	FTimerHandle SpawnTimerHandle;
	FTimerHandle FireTimerHandle;
};
