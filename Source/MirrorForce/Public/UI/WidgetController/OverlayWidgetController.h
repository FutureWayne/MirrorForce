// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MirrorForceWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseSignature);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MIRRORFORCE_API UOverlayWidgetController : public UMirrorForceWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnWinSignature OnWin;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnLoseSignature OnLose;

	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;
};
