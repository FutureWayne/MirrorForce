// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Game/MirrorForceLaneController.h"
#include "MirrorForceWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet, AMirrorForceLaneController* InMirrorForceLaneController)
		: PlayerController(InPlayerController)
		, PlayerState(InPlayerState)
		, AbilitySystemComponent(InAbilitySystemComponent)
		, AttributeSet(InAttributeSet)
		, MirrorForceLaneController(InMirrorForceLaneController) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetControllerParams")
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetControllerParams")
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetControllerParams")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetControllerParams")
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetControllerParams")
	TObjectPtr<AMirrorForceLaneController> MirrorForceLaneController = nullptr;
};

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorForceWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MirrorForceWidgetController")
	void SetWidgetControllerParams(const FWidgetControllerParams& InWCParams);

	virtual void BroadcastInitialValues();

	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "MirrorForceWidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "MirrorForceWidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "MirrorForceWidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "MirrorForceWidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "MirrorForceWidgetController")
	TObjectPtr<AMirrorForceLaneController> MirrorForceLaneController;
};
