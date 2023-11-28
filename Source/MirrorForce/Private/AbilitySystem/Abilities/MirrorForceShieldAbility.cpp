// Copyright Mirror Force


#include "AbilitySystem/Abilities/MirrorForceShieldAbility.h"

#include "Actor/MirrorForceShield.h"

void UMirrorForceShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwningActorFromActorInfo();
	SpawnParams.Instigator = Cast<APawn>(GetOwningActorFromActorInfo());

	const FVector SpawnLocation = ActorInfo->AvatarActor->GetActorLocation();
	const FRotator SpawnRotation = ActorInfo->AvatarActor->GetActorRotation();
	
	SpawnedShield = GetWorld()->SpawnActor<AMirrorForceShield>(ShieldClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedShield)
	{
		SpawnedShield->AttachToComponent(ActorInfo->AvatarActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void UMirrorForceShieldAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		SpawnedShield->Destroy();
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Input Released"));
}
