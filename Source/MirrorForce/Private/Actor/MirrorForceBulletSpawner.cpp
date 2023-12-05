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
void AMirrorForceBulletSpawner::SpawnCirclePattern(int InNumBullets, float AngleBetweenBullets, float InBulletSpeed)
{
	for (int i = 0; i < InNumBullets; i++)
	{
		const float Angle = i * AngleBetweenBullets;
		SpawnBullet(Angle, InBulletSpeed);
	}
}

void AMirrorForceBulletSpawner::SpawnMultipleCircles(int InNumBullets, float AngleBetweenBullets, float InBulletSpeed, int NumSpirals, float DelayBetweenSpirals)
{
	for (int i = 0; i <= NumSpirals; i++)
	{
		if (DelayBetweenSpirals > 0.f)
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("SpawnCirclePattern"), InNumBullets, AngleBetweenBullets, InBulletSpeed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenSpirals, false);
		}
		else
		{
			SpawnCirclePattern(InNumBullets, AngleBetweenBullets, InBulletSpeed);
		}
	}
}


//SPIRAL
void AMirrorForceBulletSpawner::SpawnSpiralPattern(int InNumBullets, float InAngleIncrement, float InBulletSpeed, float DelayBetweenBullets)
{
	this->NumBullets = InNumBullets;
	this->AngleIncrement = InAngleIncrement;
	this->BulletSpeed = InBulletSpeed;
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


void AMirrorForceBulletSpawner::SpawnBullet(float Angle, float InBulletSpeed) const
{
	const FVector StartLocation = GetActorLocation();
	if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()); Bullet != nullptr)
	{
		Bullet->SetActorLocation(StartLocation);
		const FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
		Bullet->ProjectileMovement->Velocity = Direction * InBulletSpeed;
	}
}

//HOMING
void AMirrorForceBulletSpawner::SpawnHoveringPattern(int InNumBullets, float DelayBetweenBullets, float InBulletSpeed, AActor* InPlayer)
{
	this->NumBullets = InNumBullets;
	this->CurrentBullet = 0;
	this->Player = InPlayer;
	this->BulletSpeed = InBulletSpeed;

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
