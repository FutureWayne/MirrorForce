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


//CIRCLE
void AMirrorForceBulletSpawner::SpawnCirclePattern(int numBullets, float AngleBetweenBullets, float bulletSpeed)
{
	const FVector StartLocation = GetActorLocation();

	for (int i = 0; i < numBullets; i++)
	{
		const float Angle = i * AngleBetweenBullets;
		SpawnBullet(Angle, bulletSpeed);
	}
}

void AMirrorForceBulletSpawner::SpawnMultipleCircles(int numBullets, float AngleBetweenBullets, float bulletSpeed, int NumSpirals, float DelayBetweenSpirals)
{
	for (int i = 0; i <= NumSpirals; i++)
	{
		if (DelayBetweenSpirals > 0.f)
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("SpawnCirclePattern"), numBullets, AngleBetweenBullets, bulletSpeed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenSpirals, false);
		}
		else
		{
			SpawnCirclePattern(numBullets, AngleBetweenBullets, bulletSpeed);
		}
	}
}


//SPIRAL
void AMirrorForceBulletSpawner::SpawnSpiralPattern(int numBullets, float angleIncrement, float bulletSpeed, float DelayBetweenBullets)
{
	this->NumBullets = numBullets;
	this->AngleIncrement = angleIncrement;
	this->BulletSpeed = bulletSpeed;
	this->CurrentBullet = 0;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnBulletWithTimer"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, DelayBetweenBullets, true);
}

void AMirrorForceBulletSpawner::SpawnBulletWithTimer()
{
	if (CurrentBullet < NumBullets)
	{
		SpawnBullet(CurrentBullet * AngleIncrement, BulletSpeed);
		CurrentBullet++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}


void AMirrorForceBulletSpawner::SpawnBullet(float Angle, float bulletSpeed) const
{
	const FVector StartLocation = GetActorLocation();
	if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()); Bullet != nullptr)
	{
		Bullet->SetActorLocation(StartLocation);
		const FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
		Bullet->ProjectileMovement->Velocity = Direction * bulletSpeed;
	}
}

//HOMING
void AMirrorForceBulletSpawner::SpawnHoveringPattern(int numBullets, float DelayBetweenBullets, float bulletSpeed, AActor* player)
{
	this->NumBullets = numBullets;
	this->CurrentBullet = 0;
	this->Player = player;
	this->BulletSpeed = bulletSpeed;

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnSingleHoveringBullet"));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, TimerDel, DelayBetweenBullets, true);
}

void AMirrorForceBulletSpawner::SpawnSingleHoveringBullet()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn."));
	if (CurrentBullet < NumBullets)
	{
		AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor());
		if (Bullet != nullptr)
		{
			FVector StartLocation = GetActorLocation();
			StartLocation.X += FMath::RandRange(-450, 0); // horizontal offset
			StartLocation.Y += FMath::RandRange(-1000, 1000); // depth offset
			Bullet->SetActorLocation(StartLocation);
			Bullet->ProjectileMovement->Velocity = FVector::ZeroVector;
			HoveringBullets.Add(Bullet);
		}
		CurrentBullet++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		CurrentBullet = 0;
		FTimerDelegate FireDel;
		FireDel.BindUFunction(this, FName("FireHoveringBullet"));
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireDel, 0.2f, true);
	}
}

void AMirrorForceBulletSpawner::FireHoveringBullet()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire."));
		
	if (CurrentBullet < HoveringBullets.Num())
	{
		FVector Direction = Player->GetActorLocation() - HoveringBullets[CurrentBullet]->GetActorLocation();
		Direction.Normalize();
		HoveringBullets[CurrentBullet]->ProjectileMovement->Velocity = Direction * BulletSpeed;
		CurrentBullet++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
}
