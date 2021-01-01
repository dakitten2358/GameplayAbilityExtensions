// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "AxAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AXCOMMON_API UAxAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void BindAbilityActivationToEnhancedInputComponent(UInputComponent* InputComponent);

private:


	void OnInputActionStarted(const UInputAction* InputAction);
	void OnInputActionEnded(const UInputAction* InputAction);

	void CacheInputActions();

	bool bHasCachedInputActions;

	UPROPERTY()
	TArray<const class UInputAction*> CachedInputActions;
};
