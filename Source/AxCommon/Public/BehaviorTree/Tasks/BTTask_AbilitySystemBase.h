// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AbilitySystemBase.generated.h"

UCLASS(Abstract)
class AXCOMMON_API UBTTask_AbilitySystemBase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, class UAbilitySystemComponent* AbilitySystemComp, uint8* NodeMemory);
};
