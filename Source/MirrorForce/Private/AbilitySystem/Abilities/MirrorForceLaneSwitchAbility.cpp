// Copyright Mirror Force


#include "AbilitySystem/Abilities/MirrorForceLaneSwitchAbility.h"

#include "Game/MirrorForceGameModeBase.h"
#include "Game/MirrorForceLaneController.h"

void UMirrorForceLaneSwitchAbility::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	ActivateAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	if (const AMirrorForceGameModeBase* GameMode = Cast<AMirrorForceGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		AMirrorForceLaneController* LaneController = GameMode->LaneController;
		if (LaneController)
		{
			LaneController->ChangeToNextScrollingLane();
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
