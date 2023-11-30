// Copyright Mirror Force

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MirrorForceInputConfig.h"
#include "MirrorEnhancedInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MIRRORFORCE_API UMirrorEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UMirrorForceInputConfig* InInputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
	{
		check(InInputConfig);
		
		for (const FMirrorForceInputAction& InputAction : InInputConfig->AbilityInputActions)
		{
			if (InputAction.InputAction && InputAction.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag);
				}

				if (ReleasedFunc)
				{
					BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag);
				}

				if (HeldFunc)
				{
					BindAction(InputAction.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, InputAction.InputTag);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid input action or tag in %s"), *InInputConfig->GetName());
			}
		}
	}
};
