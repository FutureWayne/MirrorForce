// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "MirrorForcePooledActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledActorDespawn, AMirrorForcePooledActor*, PooledActor);

UCLASS()
class MIRRORFORCE_API AMirrorForcePooledActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMirrorForcePooledActor();

	FOnPooledActorDespawn OnPooledActorDespawn;
	
	UFUNCTION(BlueprintCallable, Category = "Pooled Actor")
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
