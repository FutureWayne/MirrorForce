// Copyright Mirror Force


#include "AbilitySystem/MirrorAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/MirrorForceGameplayAbility.h"

void UMirrorAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		// Get the startup gameplay tag and add it to the ability spec so that it can be rebind at runtimes
		if (const UMirrorForceGameplayAbility* MirrorForceAbility = Cast<UMirrorForceGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(MirrorForceAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UMirrorAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for	(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			TryActivateAbility(Spec.Handle);
			AbilitySpecInputPressed(Spec);
		}
	}
}

void UMirrorAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for	(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
		{
			AbilitySpecInputReleased(Spec);
		}
	}
}

void UMirrorAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	for	(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}
