// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MirrorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
};
