// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_AbilitySystemBase.h"
#include "GameplayTagContainer.h"
#include "BTDecorator_HasGameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class AXCOMMON_API UBTDecorator_HasGameplayTag : public UBTDecorator_AbilitySystemBase
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(Category=Node, EditAnywhere)
	FGameplayTagContainer Tags;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, class UAbilitySystemComponent* AbilitySystemComponent, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
};
