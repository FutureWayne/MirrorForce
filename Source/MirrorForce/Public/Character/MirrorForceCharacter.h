// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/MirrorForceCharacterBase.h"
#include "MirrorForceCharacter.generated.h"

class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Blueprintable)
class AMirrorForceCharacter : public AMirrorForceCharacterBase
{
	GENERATED_BODY()

public:
	AMirrorForceCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<UNiagaraSystem> DeathEffect;

private:
	void InitAbilityActorInfo();
	void OnPlayerDead();
	void OnHealthChange(const struct FOnAttributeChangeData& OnAttributeChangeData);

	bool bIsDead = false;
	
	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
};

