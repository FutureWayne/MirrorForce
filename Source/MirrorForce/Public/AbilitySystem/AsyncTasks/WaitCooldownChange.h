// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
struct FGameplayTagContainer;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSingature, float, CooldownDuration);

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "WaitForCooldownChange")
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "WaitForCooldownChange")
	void EndTask();
	
	UPROPERTY(BlueprintAssignable, Category = "WaitForCooldownChange")
	FCooldownChangeSingature OnCooldownStarted;

	UPROPERTY(BlueprintAssignable, Category = "WaitForCooldownChange")
	FCooldownChangeSingature OnCooldownEnded;
protected:
	void OnCooldownChange(const FGameplayTag InCooldownTag, int32 NewCount) const;

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle) const;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	FGameplayTag CooldownTag;
};
