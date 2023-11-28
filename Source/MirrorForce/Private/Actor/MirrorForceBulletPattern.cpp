// Copyright Mirror Force


#include "Actor/MirrorForceBulletPattern.h"

// Sets default values for this component's properties
UMirrorForceBulletPattern::UMirrorForceBulletPattern()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UMirrorForceBulletPattern::SpawnCirclePattern(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleBetweenBullets, float BulletSpeed)
{
	FVector StartLocation = GetOwner()->GetActorLocation();

	for (int i = 0; i < NumBullets; i++)
	{
		AMirrorForcePooledBullet* Bullet = BulletPool->SpawnPooledBullet();
		if (Bullet != nullptr)
		{
			Bullet->SetActorLocation(StartLocation);
			float Angle = i * AngleBetweenBullets;
			FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
			Bullet->ProjectileMovement->Velocity = Direction * BulletSpeed;
		}
	}
}

void UMirrorForceBulletPattern::SpawnMultipleCircles(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleBetweenBullets, float BulletSpeed, int NumSpirals, float DelayBetweenSpirals)
{
	for (int i = 0; i <= NumSpirals; i++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SpawnCirclePattern"), BulletPool, NumBullets, AngleBetweenBullets, BulletSpeed);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenSpirals, false);
	}
}

void UMirrorForceBulletPattern::SpawnSpiralPattern(UMirrorForceBulletPool* BulletPool, int NumBullets, float AngleIncrement, float BulletSpeed, float DelayBetweenBullets)
{
	for (int i = 0; i < NumBullets; i++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SpawnBullet"), BulletPool, i * AngleIncrement, BulletSpeed);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenBullets, false);
	}
}

void UMirrorForceBulletPattern::SpawnBullet(UMirrorForceBulletPool* BulletPool, float Angle, float BulletSpeed)
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	AMirrorForcePooledBullet* Bullet = BulletPool->SpawnPooledBullet();
	if (Bullet != nullptr)
	{
		Bullet->SetActorLocation(StartLocation);
		FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
		Bullet->ProjectileMovement->Velocity = Direction * BulletSpeed;
	}
}


