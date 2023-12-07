// Copyright Mirror Force


#include "AbilitySystem/Abilities/MirrorForceShieldAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MirrorAttributeSet.h"
#include "Actor/MirrorForceShield.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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
		
		// Apply ongoing mana cost
		if (ManaCostEffectClass)
		{
			FGameplayEffectContextHandle EffectContextHandle = ActorInfo->AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(ManaCostEffectClass, 1.0f, EffectContextHandle);
			ManaCostEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}

		// Remove mana regen
		if (ManaRegenEffectHandle.IsValid())
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ManaRegenEffectHandle);
		}

		ShieldOn();
	}
}

void UMirrorForceShieldAbility::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (ActorInfo != nullptr && ActorInfo->AvatarActor != nullptr)
	{
		if (SpawnedShield)
		{
			SpawnedShield->Destroy();
			SpawnedShield = nullptr;
		}
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}

	// Remove ongoing mana cost
	if (ManaCostEffectHandle.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ManaCostEffectHandle);
	}

	// Apply mana regen
	if (ManaRegenEffectClass)
	{
		FGameplayEffectContextHandle EffectContextHandle = ActorInfo->AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(ManaRegenEffectClass, 1.0f, EffectContextHandle);
		ManaRegenEffectHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}

	if (SpawnedShield)
	{
		ShieldOff();
	}

}

void UMirrorForceShieldAbility::OnManaChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	// Check if mana falls below 0
	if (OnAttributeChangeData.NewValue < 0.0f)
	{
		if (SpawnedShield)
		{
			SpawnedShield->Destroy();
			SpawnedShield = nullptr;
			ShieldOff();
		}
	} 
}

void UMirrorForceShieldAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// get the ability system component attribute set
	const UMirrorAttributeSet* MirrorAttributeSet = ActorInfo->AbilitySystemComponent->GetSet<UMirrorAttributeSet>();

	// Bind on mana change delegate
	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MirrorAttributeSet->GetManaAttribute()).AddUObject(this, &UMirrorForceShieldAbility::OnManaChange);
	}
}

void UMirrorForceShieldAbility::ShieldOn()
{
	UGameplayStatics::PlaySoundAtLocation(this, ShieldOnSFX, FVector::ZeroVector);
	if (ShieldPersistAudioComponent == nullptr)
	{
		ShieldPersistAudioComponent = UGameplayStatics::SpawnSoundAtLocation(this, ShieldPersistSFX, FVector::ZeroVector);
	}
	else
	{
		ShieldPersistAudioComponent->SetPaused(false);
	}
	
}

void UMirrorForceShieldAbility::ShieldOff()
{
	UGameplayStatics::PlaySoundAtLocation(this, ShieldOffSFX, FVector::ZeroVector);
	if (ShieldPersistAudioComponent != nullptr)
	{
		ShieldPersistAudioComponent->SetPaused(true);
	}
}
