// Copyright Mirror Force


#include "Actor/MirrorForceBulletSpawner.h"
#include "Actor/MirrorForceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
AMirrorForceBulletSpawner::AMirrorForceBulletSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletPool = CreateDefaultSubobject<UMirrorForceActorPool>(TEXT("Bullet Pool"));
}

void AMirrorForceBulletSpawner::SpawnCirclePattern(int NumBullets, float AngleBetweenBullets, float BulletSpeed)
{
	const FVector StartLocation = GetActorLocation();

	for (int i = 0; i < NumBullets; i++)
	{
		AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor());
		if (Bullet != nullptr)
		{
			Bullet->SetActorLocation(StartLocation);
			const float Angle = i * AngleBetweenBullets;
			FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
			Bullet->ProjectileMovement->Velocity = Direction * BulletSpeed;
		}
	}
}

void AMirrorForceBulletSpawner::SpawnMultipleCircles(int NumBullets, float AngleBetweenBullets, float BulletSpeed, int NumSpirals, float DelayBetweenSpirals)
{
	for (int i = 0; i <= NumSpirals; i++)
	{
		if (DelayBetweenSpirals > 0.f)
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("SpawnCirclePattern"), BulletPool, NumBullets, AngleBetweenBullets, BulletSpeed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenSpirals, false);
		}
		else
		{
			SpawnCirclePattern(NumBullets, AngleBetweenBullets, BulletSpeed);
		}
	}
}

void AMirrorForceBulletSpawner::SpawnSpiralPattern(int NumBullets, float AngleIncrement, float BulletSpeed, float DelayBetweenBullets)
{
	for (int i = 0; i < NumBullets; i++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		if (DelayBetweenBullets > 0.f)
		{
			TimerDel.BindUFunction(this, FName("SpawnBullet"), BulletPool, i * AngleIncrement, BulletSpeed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenBullets, false);
		}
		else
		{
			SpawnBullet(i * AngleIncrement, BulletSpeed);
		}
	}
}

void AMirrorForceBulletSpawner::SpawnBullet(float Angle, float BulletSpeed) const
{
	const FVector StartLocation = GetActorLocation();
	if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()); Bullet != nullptr)
	{
		Bullet->SetActorLocation(StartLocation);
		const FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
		Bullet->ProjectileMovement->Velocity = Direction * BulletSpeed;
	}
}


