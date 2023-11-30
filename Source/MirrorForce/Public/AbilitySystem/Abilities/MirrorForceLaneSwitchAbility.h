// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MirrorForceGameplayAbility.h"
#include "MirrorForceLaneSwitchAbility.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceLaneSwitchAbility : public UMirrorForceGameplayAbility
{
	GENERATED_BODY()

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};
