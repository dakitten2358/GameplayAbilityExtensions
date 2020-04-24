// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AxGameplayAbility.generated.h"

/**
 *
 */
UCLASS(Abstract)
class AXCOMMON_API UAxGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// must be implemented in derived classes!
	virtual int32 GetAbilityInputID() const;
};
