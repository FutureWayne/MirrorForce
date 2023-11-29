// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Actor/ActorPool/MirrorForcePooledActor.h"
#include "MirrorForceActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MIRRORFORCE_API UMirrorForceActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMirrorForceActorPool();

	UFUNCTION(BlueprintCallable, Category = "Actor Pool")
	AMirrorForcePooledActor* SpawnPooledActor();
	
	UFUNCTION()
	void OnPooledActorDespawn(AMirrorForcePooledActor* PooledActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	TSubclassOf<AMirrorForcePooledActor> PooledActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	int PoolSize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	float PooledActorLifeSpan = 0.0f;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AMirrorForcePooledActor*> ActorPool;
	
	TArray<int> SpawnedPoolIndexes;
};
