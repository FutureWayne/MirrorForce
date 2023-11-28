// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Actor/MirrorForceBulletBase.h"
#include "MirrorForcePooledBullet.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledBulletDespawn, AMirrorForcePooledBullet*, PooledBullet);

UCLASS()
class MIRRORFORCE_API AMirrorForcePooledBullet : public AMirrorForceBulletBase
{
	GENERATED_BODY()
	
public:
	AMirrorForcePooledBullet();

	FOnPooledBulletDespawn OnPooledBulletDespawn;

	UPROPERTY(EditAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable, Category = "Pooled Bullet")
	void Deactivate();

	void SetActive(bool isActive);
	void SetLifeSpan(float LifeTime);
	void SetPoolIndex(int index);


	bool IsActive();
	int GetPoolIndex();

protected:
	bool Active;
	float LifeSpan = 0.0f;
	int PoolIndex;

	FTimerHandle LifeSpanTimer;


};
