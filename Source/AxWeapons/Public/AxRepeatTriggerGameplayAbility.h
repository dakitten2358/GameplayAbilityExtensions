// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "AxGameplayAbility.h"
#include "AxRepeatTriggerGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AXWEAPONS_API UAxRepeatTriggerGameplayAbility : public UAxGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Trigger(int repeatCount = 0);	
};
