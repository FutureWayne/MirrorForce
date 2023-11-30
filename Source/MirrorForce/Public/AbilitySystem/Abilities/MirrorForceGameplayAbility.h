// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MirrorForceGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
