// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MirrorForceGameplayAbility.h"
#include "MirrorForceShieldAbility.generated.h"

class AMirrorForceShield;
/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceShieldAbility : public UMirrorForceGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MirrorForce|Shield")
	TSubclassOf<AMirrorForceShield> ShieldClass;

private:
	TObjectPtr<AMirrorForceShield> SpawnedShield;
};
