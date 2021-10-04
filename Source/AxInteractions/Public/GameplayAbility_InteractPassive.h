#pragma once

#include "CoreMinimal.h"
#include "AxGameplayAbility.h"
#include "GameplayAbility_InteractPassive.generated.h"

/**
 * 
 */
UCLASS()
class AXINTERACTIONS_API UGameplayAbility_InteractPassive : public UAxGameplayAbility
{
	GENERATED_BODY()

private:
	typedef UGameplayAbility_InteractPassive Self;
	static const FName WaitInteractionTargetDataTaskName;

	UPROPERTY()
	class UAxTask_WaitInteractionTargetData* WaitInteractionTargetDataTask;

	UPROPERTY()
	class UAbilityTask_WaitGameplayEvent* WaitInteractionCompleteEventTask;

	UPROPERTY()
	class UAbilityTask_WaitInputPress* WaitInteractionInputPressed;

	UPROPERTY()
	class UAbilityTask_WaitInputRelease* WaitInteractionInputReleased;

	UPROPERTY()
	class AAxInteractionTargetActor* InteractionTargetActor;

	FGameplayAbilityTargetDataHandle CurrentInteractionTarget;

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class AAxInteractionTargetActor> InteractionTargetActorClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag InteractionCompleteTag;

public:
	UGameplayAbility_InteractPassive(const FObjectInitializer& ObjectInitializer = FObjectInitializer());

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnFoundNewInteractionTarget(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnLostInteractionTarget(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnInteractionComplete(FGameplayEventData Payload);

	UFUNCTION()
	void OnInteractionInputPressed(float TimeWaited);

	UFUNCTION()
	void OnInteractionInputReleased(float TimeWaited);

	void SetNewInteractionTarget(const FGameplayAbilityTargetDataHandle& Data);
	void ClearInteractionTarget();
	AActor* GetInteractionInterface(const FGameplayAbilityTargetDataHandle& Data);

	void StartWaitForInteractionInputPressed();
	void StartWaitForInteractionInputReleased();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnFoundNewInteractionTarget(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnLostInteractionTarget(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnInteractionStart(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnInteractionComplete(FGameplayEventData Payload);
};
