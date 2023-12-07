// Copyright Mirror Force


#include "Actor/ActorPool/MirrorForcePooledActor.h"

AMirrorForcePooledActor::AMirrorForcePooledActor()
{
}

void AMirrorForcePooledActor::SetActive(bool IsActive)
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &AMirrorForcePooledActor::Deactivate, LifeSpan, false);
}

void AMirrorForcePooledActor::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void AMirrorForcePooledActor::SetPoolIndex(int index)
{
	PoolIndex = index;
}

void AMirrorForcePooledActor::Deactivate()
{
	SetActorLocation(FVector(0,0,0));
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledActorDespawn.Broadcast(this);
}

bool AMirrorForcePooledActor::IsActive()
{
	return Active;
}

int AMirrorForcePooledActor::GetPoolIndex()
{
	return PoolIndex;
}
