// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorators/BTDecorator_AbilitySystemBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "AxCommonDebug.h"

bool UBTDecorator_AbilitySystemBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && AIController->GetPawn() && AIController->GetPawn()->Implements<UAbilitySystemInterface>())
	{
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(AIController->GetPawn());
		if (AbilitySystemInterface)
		{
			UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
			if (AbilitySystemComponent)
			{
				return CalculateRawConditionValue(OwnerComp, AbilitySystemComponent, NodeMemory);
			}
			else
			{
				UE_LOG(LogAxAI, Error, TEXT("UBTDecorator_AbilitySystemBase:  Found a null AbilitySystemComponent"));
			}
		}
		else
		{
			UE_LOG(LogAxAI, Error, TEXT("UBTDecorator_AbilitySystemBase:  Failed to find IAbiltiySystemInterface on controller!"));
		}
	}
	else
	{
		UE_LOG(LogAxAI, Error, TEXT("UBTDecorator_AbilitySystemBase:  Failed to get AIController, it's pawn or it's pawn doesn't implement IAbilitySystemInterface"))
	}
	return false;
}

bool UBTDecorator_AbilitySystemBase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, UAbilitySystemComponent* AbilitySystemComponent, uint8* NodeMemory) const
{
	return true;
}