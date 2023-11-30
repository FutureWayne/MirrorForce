// Copyright Mirror Force


#include "Actor/ActorPool/MirrorForceActorPool.h"

// Sets default values for this component's properties
UMirrorForceActorPool::UMirrorForceActorPool()
{
}

void UMirrorForceActorPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledActorClass != nullptr)
	{
		UWorld* const World = GetWorld();

		if (World != nullptr)
		{ 
			for (int i = 0; i < PoolSize; i++)
			{
				AMirrorForcePooledActor* PoolableActor = World->SpawnActor<AMirrorForcePooledActor>(PooledActorClass, FVector().ZeroVector, FRotator().ZeroRotator);

				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActive(false);
					PoolableActor->SetPoolIndex(i);
					PoolableActor->OnPooledActorDespawn.AddDynamic(this, &UMirrorForceActorPool::OnPooledActorDespawn);
					ActorPool.Add(PoolableActor);
				}
			}
		}
	}
}

AMirrorForcePooledActor* UMirrorForceActorPool::SpawnPooledActor()
{
	for (AMirrorForcePooledActor* PooledActor : ActorPool)
	{
		if (PooledActor != nullptr && !PooledActor->IsActive())
		{
			PooledActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PooledActor->SetLifeSpan(PooledActorLifeSpan);
			PooledActor->SetActive(true);
			SpawnedPoolIndexes.Add(PooledActor->GetPoolIndex());

			return PooledActor;
		}
	}

	//CHANGE
	if (SpawnedPoolIndexes.Num() > 0)
	{
		int PooledActorIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledActorIndex);
		AMirrorForcePooledActor* PooledActor = ActorPool[PooledActorIndex];

		if (PooledActor != nullptr)
		{
			PooledActor->SetActive(false);

			PooledActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
			PooledActor->SetLifeSpan(PooledActorLifeSpan);
			PooledActor->SetActive(true);
			SpawnedPoolIndexes.Add(PooledActor->GetPoolIndex());

			return PooledActor;
		}
	}
	return nullptr;
}

void UMirrorForceActorPool::OnPooledActorDespawn(AMirrorForcePooledActor* PooledActor)
{
	SpawnedPoolIndexes.Remove(PooledActor->GetPoolIndex());
}
