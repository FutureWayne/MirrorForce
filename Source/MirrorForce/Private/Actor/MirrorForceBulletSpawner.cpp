// Copyright Mirror Force


#include "Actor/MirrorForceBulletSpawner.h"
#include "Actor/MirrorForceProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
AMirrorForceBulletSpawner::AMirrorForceBulletSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletPool = CreateDefaultSubobject<UMirrorForceActorPool>(TEXT("Bullet Pool"));
} 

void AMirrorForceBulletSpawner::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(this, 0);
}

//CIRCLE
void AMirrorForceBulletSpawner::SpawnCirclePattern(float MaxAngle, float AngleBetweenBullets, float InBulletSpeed, const FVector& Offset)
{
	//FVector PlayerDirection = Player->GetActorLocation() - GetActorLocation();
	//PlayerDirection.Normalize();
	//float StartAngle = FMath::Atan2(PlayerDirection.Y, PlayerDirection.X) -(MaxAngle / 2.0f * PI / 180.0f);;

	FVector FixedDirection = FVector(1, 0, 0);
	FixedDirection.Normalize();
	float StartAngle = FMath::Atan2(FixedDirection.Y, FixedDirection.X) - (MaxAngle / 2.0f * PI / 180.0f);

	int InNumBullets = FMath::RoundToInt(MaxAngle / AngleBetweenBullets);
	for (int i = 0; i < InNumBullets; i++)
	{
		const float Angle = StartAngle + i * AngleBetweenBullets * PI / 180.0f;
		const FVector StartLocation = GetActorLocation() + Offset;
		if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()); Bullet != nullptr)
		{
			Bullet->SetActorLocation(StartLocation);
			const FVector Direction = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
			Bullet->ProjectileMovement->Velocity = Direction * InBulletSpeed;
		}
	}
	UGameplayStatics::SpawnSoundAtLocation(this, FireSFX, GetActorLocation() + Offset);
}


void AMirrorForceBulletSpawner::SpawnMultipleCircles(float MaxAngle, float AngleBetweenBullets, float InBulletSpeed, int NumSpirals, float DelayBetweenSpirals, const FVector& Offset)
{
	for (int i = 0; i <= NumSpirals; i++)
	{
		if (DelayBetweenSpirals > 0.f)
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("SpawnCirclePattern"), MaxAngle, AngleBetweenBullets, InBulletSpeed, Offset);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, i * DelayBetweenSpirals, false);
		}
		else
		{
			SpawnCirclePattern(MaxAngle, AngleBetweenBullets, InBulletSpeed, Offset);
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
	UGameplayStatics::SpawnSoundAtLocation(this, FireSFX, GetActorLocation());
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
void AMirrorForceBulletSpawner::SpawnHoveringPattern(int InNumBullets, float DelayBetweenBullets, float InBulletSpeed)
{
	this->NumBullets = InNumBullets;
	this->CurrentBullet = 0;
	this->BulletSpeed = InBulletSpeed;

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnSingleHoveringBullet"));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, TimerDel, DelayBetweenBullets, true);
	UGameplayStatics::SpawnSoundAtLocation(this, FireSFX, GetActorLocation());
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

void AMirrorForceBulletSpawner::SpawnFanPattern(int InNumLines, int InNumBulletsPerLine, float InRotateSpeed)
{
	this->NumLines = InNumLines;
	this->NumBulletsPerLine = InNumBulletsPerLine;
	this->CurrentLine = 0;
	this->CurrentBullet = 0;
	this->RotateSpeed = InRotateSpeed;
	const float AngleBetweenLines = 2 * PI / InNumLines;
	const float TotalLineLength = 2500; // Change this value to control the total length of the line

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("SpawnSingleBulletInLine"));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, TimerDel, 0.2f, true);
}

void AMirrorForceBulletSpawner::SpawnSingleBulletInLine()
{
	if (CurrentLine < NumLines)
	{
		const float LineAngle = CurrentLine * (2 * PI / NumLines);
		if (CurrentBullet < NumBulletsPerLine)
		{
			const float Distance = CurrentBullet * (2500 / NumBulletsPerLine);
			SpawnBulletAtDistance(LineAngle, Distance);
			CurrentBullet++;
		}
		else
		{
			CurrentBullet = 0;
			CurrentLine++;
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &AMirrorForceBulletSpawner::StartRotate, 0.01f, true);
	}
}

void AMirrorForceBulletSpawner::SpawnBulletAtDistance(float Angle, float Distance)
{
	const FVector StartLocation = GetActorLocation() + Distance * FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.f);
	if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()))
	{
		Bullet->SetActorLocation(StartLocation);
		Bullet->ProjectileMovement->Velocity = FVector::ZeroVector;
		Bullet->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}
void AMirrorForceBulletSpawner::StartRotate()
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotateSpeed;
	SetActorRotation(NewRotation);
}

void AMirrorForceBulletSpawner::SpawnCrossPattern(float InitialSpeed, float BurstSpeed, float Delay)
{
	FVector StartLocation = GetActorLocation();

	for (int i = 0; i < 4; i++)
	{
		if (AMirrorForceProjectile* Bullet = Cast<AMirrorForceProjectile>(BulletPool->SpawnPooledActor()); Bullet != nullptr)
		{
			Bullet->SetActorLocation(StartLocation);
			Bullet->ProjectileMovement->Velocity = FVector(1, 0, 0) * InitialSpeed;

			FTimerHandle TimerHandle;
			FTimerDelegate TimerDel;
			TimerDel.BindUFunction(this, FName("GenerateCrossBullet"), Bullet, i, BurstSpeed);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Delay, false);
		}
	}
	UGameplayStatics::SpawnSoundAtLocation(this, FireSFX, GetActorLocation());
}

void AMirrorForceBulletSpawner::SpawnMultipleCross(int NumBalls, float InitialSpeed, float BurstSpeed, float Delay, float SpawnDelay)
{
	for (int j = 0; j < NumBalls; j++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("SpawnCrossPattern"), InitialSpeed, BurstSpeed, Delay);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, j * SpawnDelay, false);
	}
}

void AMirrorForceBulletSpawner::GenerateCrossBullet(AMirrorForceProjectile* Bullet, int i, float BurstSpeed)
{
	FVector Direction;
	switch (i)
	{
	case 0: Direction = FVector(FMath::Sqrt(2.0f) / 2, FMath::Sqrt(2.0f) / 2, 0); break; 
	case 1: Direction = FVector(-FMath::Sqrt(2.0f) / 2, FMath::Sqrt(2.0f) / 2, 0); break; 
	case 2: Direction = FVector(-FMath::Sqrt(2.0f) / 2, -FMath::Sqrt(2.0f) / 2, 0); break; 
	case 3: Direction = FVector(FMath::Sqrt(2.0f) / 2, -FMath::Sqrt(2.0f) / 2, 0); break;
	}
	if (Bullet && Bullet->ProjectileMovement)
	{
		Bullet->ProjectileMovement->Velocity = Direction * BurstSpeed;
	}
}
