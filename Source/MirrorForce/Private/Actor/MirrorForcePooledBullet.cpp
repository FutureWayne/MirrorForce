// Copyright Mirror Force


#include "Actor/MirrorForcePooledBullet.h"

AMirrorForcePooledBullet::AMirrorForcePooledBullet()
{
}

void AMirrorForcePooledBullet::SetActive(bool IsActive)
{
	Active = IsActive;
	SetActorHiddenInGame(!IsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &AMirrorForcePooledBullet::Deactivate, LifeSpan, false);
}

void AMirrorForcePooledBullet::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void AMirrorForcePooledBullet::SetPoolIndex(int index)
{
	PoolIndex = index;
}

void AMirrorForcePooledBullet::Deactivate()
{
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledBulletDespawn.Broadcast(this);
}

bool AMirrorForcePooledBullet::IsActive()
{
	return Active;
}

int AMirrorForcePooledBullet::GetPoolIndex()
{
	return PoolIndex;
}
