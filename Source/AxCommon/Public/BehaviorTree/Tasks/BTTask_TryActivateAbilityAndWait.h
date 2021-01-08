// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_AbilitySystemBase.h"
#include "BTTask_TryActivateAbilityAndWait.generated.h"

/**
 * 
 */
UCLASS()
class AXCOMMON_API UBTTask_ActivateAbilityAndWaitEvent : public UBTTask_AbilitySystemBase
{
	GENERATED_BODY()
protected:
	struct FBTActivateAbilityAndWaitEventMemory
	{
		FDelegateHandle GameplayEventDelegateHandle;
		bool Completed;
	};

public:
	UPROPERTY(Category = Node, EditAnywhere)
	FGameplayTagContainer AbilityTags;

	UPROPERTY(Category = Node, EditAnywhere)
	FGameplayTagContainer WaitForEventTags;

	UBTTask_ActivateAbilityAndWaitEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer());
	virtual uint16 GetInstanceMemorySize() const override { return (uint16)sizeof(FBTActivateAbilityAndWaitEventMemory); }

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, UAbilitySystemComponent* AbilitySystemComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void OnGameplayTagEvent(FGameplayTag GameplayTag, const FGameplayEventData* EventData, FBTActivateAbilityAndWaitEventMemory* Instance);
};
