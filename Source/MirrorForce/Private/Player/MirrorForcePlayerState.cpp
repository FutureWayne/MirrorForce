// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MirrorForcePlayerState.h"
#include "AbilitySystem/MirrorAbilitySystemComponent.h"
#include "AbilitySystem/MirrorAttributeSet.h"


AMirrorForcePlayerState::AMirrorForcePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMirrorAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UMirrorAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AMirrorForcePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
