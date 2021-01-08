// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Tasks/BTTask_AbilitySystemBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AIController.h"
#include "AxCommonDebug.h"

EBTNodeResult::Type UBTTask_AbilitySystemBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				return ExecuteTask(OwnerComp, AbilitySystemComponent, NodeMemory);
			}
			else
			{
				UE_LOG(LogAxCommon, Error, TEXT("UBTTask_AbilitySystemBase:  Found a null AbilitySystemComponent"));
			}
		}
		else
		{
			UE_LOG(LogAxCommon, Error, TEXT("UBTTask_AbilitySystemBase:  Failed to find IAbiltiySystemInterface on controller!"));
		}
	}
	else
	{
		UE_LOG(LogAxCommon, Error, TEXT("UBTTask_AbilitySystemBase:  Failed to get AIController, it's pawn or it's pawn doesn't implement IAbilitySystemInterface"))
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_AbilitySystemBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UAbilitySystemComponent* AbilitySystemComp, uint8* NodeMemory)
{
	return EBTNodeResult::Succeeded;
}
