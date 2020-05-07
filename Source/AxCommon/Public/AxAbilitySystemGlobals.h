#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectTypes.h"
#include "AxAbilitySystemGlobals.generated.h"

// DefaultGame.ini
//
// [/Script/GameplayAbilities.AbilitySystemGlobals]
// AbilitySystemGlobalsClassName = "/Script/AxCommon.AxAbilitySystemGlobals"

UCLASS()
class AXCOMMON_API UAxAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
