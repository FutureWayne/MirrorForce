// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorPool/MirrorForceActorPool.h"
#include "ActorPool/MirrorForcePooledActor.h"
#include "Actor/MirrorForceProjectile.h"
#include "MirrorForceBulletSpawner.generated.h"


UCLASS()
class MIRRORFORCE_API AMirrorForceBulletSpawner : public AMirrorForcePooledActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	AMirrorForceBulletSpawner();

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnCirclePattern(float MaxAngle, float AngleBetweenBullets, float InBulletSpeed, const FVector& Offset);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnMultipleCircles(float MaxAngle, float AngleBetweenBullets, float InBulletSpeed, int NumSpirals, float DelayBetweenSpirals, const FVector& Offset);
	
	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnSpiralPattern(int InNumBullets, float InAngleIncrement, float InBulletSpeed, float DelayBetweenBullets);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnBullet(float Angle, float InBulletSpeed) const;

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnHoveringPattern(int InNumBullets, float DelayBetweenBullets,float InBulletSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnFanPattern(int InNumLines, int InNumBulletsPerLine, float InRotateSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnMultipleCross(int NumBalls, float InitialSpeed, float BurstSpeed, float Delay, float SpawnDelay);

	UFUNCTION()
	void GenerateCrossBullet(AMirrorForceProjectile* Bullet, int i, float BurstSpeed);

	UFUNCTION(BlueprintCallable, Category = "Bullet Pattern")
	void SpawnCrossPattern(float InitialSpeed, float BurstSpeed, float Delay);

	UFUNCTION()
	void SpawnSingleBulletInLine();

	UFUNCTION()
	void SpawnBulletWithTimer();

	UFUNCTION()
	void SpawnBulletAtDistance(float Angle, float Distance);

	UFUNCTION()
	void StartRotate();

	UFUNCTION()
	void FireHoveringBullet();

	UFUNCTION()
	void SpawnSingleHoveringBullet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Pattern")
	TObjectPtr<UMirrorForceActorPool> BulletPool;

	virtual void BeginPlay() override;

private:
	int CurrentBullet = 0;
	int NumBullets;
	int NumLines;
	int NumBulletsPerLine;
	int CurrentLine;
	float AngleIncrement;
	float BulletSpeed;
	float RotateSpeed;
	
	TArray<AMirrorForceProjectile*> HoveringBullets;
	APawn* Player;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle FireTimerHandle;
	FTimerHandle RotationTimerHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FireSFX;
};
