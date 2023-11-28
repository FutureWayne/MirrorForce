// Copyright Mirror Force


#include "AbilitySystem/Abilities/MirrorForceProjectileAbility.h"

#include "Actor/MirrorForceProjectile.h"

void UMirrorForceProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FTransform SpawnTransform = ActorInfo->AvatarActor->GetActorTransform();
	AMirrorForceProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMirrorForceProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	Projectile->FinishSpawning(SpawnTransform);
}
