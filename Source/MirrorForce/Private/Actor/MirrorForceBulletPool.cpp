// Copyright Mirror Force


#include "Actor/MirrorForceBulletPool.h"

// Sets default values for this component's properties
UMirrorForceBulletPool::UMirrorForceBulletPool()
{
}

void UMirrorForceBulletPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledBulletSubclass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{ 
			for (int i = 0; i < PoolSize; i++)
			{
				AMirrorForcePooledBullet* PoolableBullet = World->SpawnActor<AMirrorForcePooledBullet>(PooledBulletSubclass, FVector().ZeroVector, FRotator().ZeroRotator);

				if (PoolableBullet != nullptr)
				{
					PoolableBullet->SetActive(false);
					PoolableBullet->SetPoolIndex(i);
					PoolableBullet->OnPooledBulletDespawn.AddDynamic(this, &UMirrorForceBulletPool::OnPooledBulletDespawn);
					BulletPool.Add(PoolableBullet);
				}
			}
		}
	}
}

AMirrorForcePooledBullet* UMirrorForceBulletPool::SpawnPooledBullet()
{
	for (AMirrorForcePooledBullet* PoolableBullet : BulletPool)
	{
		if (PoolableBullet != nullptr && !PoolableBullet->IsActive())
		{
			PoolableBullet->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableBullet->SetLifeSpan(PooledBulletLifeSpan);
			PoolableBullet->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableBullet->GetPoolIndex());

			return PoolableBullet;
		}
	}

	//CHANGE
	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledBulletIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledBulletIndex);
		AMirrorForcePooledBullet* PoolableBullet = BulletPool[PooledBulletIndex];

		if (PoolableBullet != nullptr)
		{
			PoolableBullet->SetActive(false);

			PoolableBullet->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PoolableBullet->SetLifeSpan(PooledBulletLifeSpan);
			PoolableBullet->SetActive(true);
			SpawnedPoolIndexes.Add(PoolableBullet->GetPoolIndex());

			return PoolableBullet;
		}
	}
	return nullptr;
}


void UMirrorForceBulletPool::OnPooledBulletDespawn(AMirrorForcePooledBullet* PooledBullet)
{
	SpawnedPoolIndexes.Remove(PooledBullet->GetPoolIndex());
}



