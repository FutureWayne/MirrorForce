// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/MirrorForcePooledBullet.h"
#include "MirrorForceBulletPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRRORFORCE_API UMirrorForceBulletPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMirrorForceBulletPool();

	UFUNCTION(BlueprintCallable, Category = "Bullet Pool")
	AMirrorForcePooledBullet* SpawnPooledBullet();

	UPROPERTY(EditAnywhere, Category = "Bullet Pool")
	TSubclassOf<class AMirrorForcePooledBullet> PooledBulletSubclass;

	UPROPERTY(EditAnywhere, Category = "Bullet Pool")
	int PoolSize = 20;

	UPROPERTY(EditAnywhere, Category = "Bullet Pool")
	float PooledBulletLifeSpan = 0.0f;

	UFUNCTION()
	void OnPooledBulletDespawn(AMirrorForcePooledBullet* PooledBullet);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<AMirrorForcePooledBullet*> BulletPool;
	TArray<int> SpawnedPoolIndexes;

		
};
