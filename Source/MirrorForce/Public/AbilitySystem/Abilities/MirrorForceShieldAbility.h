// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MirrorForceGameplayAbility.h"
#include "MirrorForceShieldAbility.generated.h"

class AMirrorForceShield;
/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceShieldAbility : public UMirrorForceGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	void OnManaChange(const FOnAttributeChangeData& OnAttributeChangeData);
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void ShieldOn();

	void ShieldOff();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MirrorForce|Shield")
	TSubclassOf<AMirrorForceShield> ShieldClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MirrorForce|Shield")
	TSubclassOf<UGameplayEffect> ManaCostEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MirrorForce|Shield")
	TSubclassOf<UGameplayEffect> ManaRegenEffectClass;
	
private:
	TObjectPtr<AMirrorForceShield> SpawnedShield;

	FActiveGameplayEffectHandle ManaCostEffectHandle;

	FActiveGameplayEffectHandle ManaRegenEffectHandle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ShieldOnSFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ShieldOffSFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ShieldPersistSFX;

	UPROPERTY()
	UAudioComponent* ShieldPersistAudioComponent;
};
