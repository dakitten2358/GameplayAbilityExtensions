// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AbilitySystemBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AXCOMMON_API UBTDecorator_AbilitySystemBase : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, class UAbilitySystemComponent* AbilitySystemComponent, uint8* NodeMemory) const;
};
