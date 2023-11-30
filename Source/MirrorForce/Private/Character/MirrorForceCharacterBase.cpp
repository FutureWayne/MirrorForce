// Copyright Mirror Force

#include "Character/MirrorForceCharacterBase.h"

#include "AbilitySystem/MirrorAbilitySystemComponent.h"


AMirrorForceCharacterBase::AMirrorForceCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* AMirrorForceCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMirrorForceCharacterBase::AddCharacterAbilities() const
{
	UMirrorAbilitySystemComponent* MirrorASC = CastChecked<UMirrorAbilitySystemComponent>(GetAbilitySystemComponent());

	MirrorASC->AddCharacterAbilities(StartupAbilities);
}
