// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "Game/MirrorForceLaneController.h"
#include "GameFramework/HUD.h"
#include "MirrorForceHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UMirrorForceUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class MIRRORFORCE_API AMirrorForceHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class UMirrorForceUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet
	                 * AttributeSet, AMirrorForceLaneController* MirrorForceLaneController);

private:
	UPROPERTY(EditAnywhere, Category = "MirrorForceHUD")
	TSubclassOf<UMirrorForceUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere, Category = "MirrorForceHUD")
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
