// Copyright Mirror Force


#include "UI/WidgetController/MirrorForceWidgetController.h"

void UMirrorForceWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWCParams)
{
	PlayerController = InWCParams.PlayerController;
	PlayerState = InWCParams.PlayerState;
	AbilitySystemComponent = InWCParams.AbilitySystemComponent;
	AttributeSet = InWCParams.AttributeSet;
	MirrorForceLaneController = InWCParams.MirrorForceLaneController;
}

void UMirrorForceWidgetController::BroadcastInitialValues()
{
}

void UMirrorForceWidgetController::BindCallbacksToDependencies()
{
}
