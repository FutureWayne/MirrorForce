// Copyright Mirror Force


#include "Input/MirrorForceInputConfig.h"

const UInputAction* UMirrorForceInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InAbilityTag,
	bool bLogNotFound) const
{
	for (const FMirrorForceInputAction& InputAction : AbilityInputActions)
	{
		if (InputAction.InputTag.MatchesTag(InAbilityTag))
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability tag %s not found in input config %s"), *InAbilityTag.ToString(),
			*GetName());
	}

	return nullptr;
}
