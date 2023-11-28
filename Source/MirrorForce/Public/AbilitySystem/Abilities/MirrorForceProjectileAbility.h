// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MirrorForceGameplayAbility.h"
#include "MirrorForceProjectileAbility.generated.h"

class AMirrorForceProjectile;

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceProjectileAbility : public UMirrorForceGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MirrorForce|Projectile")
	TSubclassOf<AMirrorForceProjectile> ProjectileClass;
};
