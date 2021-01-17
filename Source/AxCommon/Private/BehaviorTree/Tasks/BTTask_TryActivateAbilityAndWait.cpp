// (C) 2021 Jay Murray


#include "BehaviorTree/Tasks/BTTask_TryActivateAbilityAndWait.h"
#include "AxCommonDebug.h"
#include "AbilitySystemComponent.h"

UBTTask_ActivateAbilityAndWaitEvent::UBTTask_ActivateAbilityAndWaitEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ActivateAbilityAndWaitEvent::ExecuteTask(UBehaviorTreeComponent& OwnerComp, UAbilitySystemComponent* AbilitySystemComp, uint8* NodeMemory)
{
	// get our memory
	FBTActivateAbilityAndWaitEventMemory* Instance = CastInstanceNodeMemory<FBTActivateAbilityAndWaitEventMemory>(NodeMemory);
	Instance->Completed = false;
	
	// create a delegate for watching gameplay events
	auto Delegate = FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UBTTask_ActivateAbilityAndWaitEvent::OnGameplayTagEvent, Instance);
	Instance->GameplayEventDelegateHandle = AbilitySystemComp->AddGameplayEventTagContainerDelegate(WaitForEventTags, Delegate);
	Instance->AbilitySystemComponent = AbilitySystemComp;
	
	// try and activate the ability
	const bool bAllowRemoteActivation = true;
	if (!AbilitySystemComp->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation))
	{
		AbilitySystemComp->RemoveGameplayEventTagContainerDelegate(WaitForEventTags, Instance->GameplayEventDelegateHandle);
		UE_LOG(LogAxAI, Error, TEXT("Failed to activate ability by tag"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ActivateAbilityAndWaitEvent::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTActivateAbilityAndWaitEventMemory* Instance = CastInstanceNodeMemory<FBTActivateAbilityAndWaitEventMemory>(NodeMemory);
	if (!Instance->Completed)
	{
		// stop listening
		Instance->AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(WaitForEventTags, Instance->GameplayEventDelegateHandle);

		// cancel any abilities we started
		TArray<FGameplayAbilitySpec*> AbilitiesToCancel;
		Instance->AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, AbilitiesToCancel);
		for (FGameplayAbilitySpec* AbilityToCancel : AbilitiesToCancel)
			Instance->AbilitySystemComponent->CancelAbility(AbilityToCancel->Ability);
		
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_ActivateAbilityAndWaitEvent::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// get our memory
	FBTActivateAbilityAndWaitEventMemory* Instance = CastInstanceNodeMemory<FBTActivateAbilityAndWaitEventMemory>(NodeMemory);
	if (Instance->Completed)
	{
		Instance->AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(WaitForEventTags, Instance->GameplayEventDelegateHandle);
		FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
	}
}

void UBTTask_ActivateAbilityAndWaitEvent::OnGameplayTagEvent(FGameplayTag GameplayTag, const FGameplayEventData* EventData, FBTActivateAbilityAndWaitEventMemory* Instance)
{
	Instance->Completed = true;
}
