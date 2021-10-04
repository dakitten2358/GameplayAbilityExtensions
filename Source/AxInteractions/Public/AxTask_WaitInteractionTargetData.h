#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AxTask_WaitInteractionTargetData.generated.h"

class AAxInteractionTargetActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitInteractableTargetDelegate, const FGameplayAbilityTargetDataHandle&, Data);

UCLASS()
class AXINTERACTIONS_API UAxTask_WaitInteractionTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate FoundNewInteractionTarget;

	UPROPERTY(BlueprintAssignable)
	FWaitInteractableTargetDelegate LostInteractionTarget;

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static UAxTask_WaitInteractionTargetData* WaitForInteractionTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<AAxInteractionTargetActor> Class, float TimerPeriod = 0.1f, bool bShowDebug = false);

	static UAxTask_WaitInteractionTargetData* WaitForInteractionTargetDataWithActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AAxInteractionTargetActor* InteractionTargetActor, float TimerPeriod = 0.1f, bool bShowDebug = false);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

/**
*	Requirements for using Begin/Finish SpawningActor functionality:
*		-Have a parameters named 'Class' in your Proxy factor function (E.g., WaitTargetdata)
*		-Have a function named BeginSpawningActor w/ the same Class parameter
*			-This function should spawn the actor with SpawnActorDeferred and return true/false if it spawned something.
*		-Have a function named FinishSpawningActor w/ an AActor* of the class you spawned
*			-This function *must* call ExecuteConstruction + PostActorConstruction
*/

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	bool BeginSpawningActor(UGameplayAbility* OwningAbility, TSubclassOf<AAxInteractionTargetActor> Class, AAxInteractionTargetActor*& SpawnedActor);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Abilities")
	void FinishSpawningActor(UGameplayAbility* OwningAbility, AAxInteractionTargetActor* SpawnedActor);

protected:
	float TimerPeriod;
	FTimerHandle TimerHandle;

	void UpdateInteractionTarget();

	void InitializeTargetActor(AAxInteractionTargetActor* SpawnedActor);
	void FinalizeTargetActor(AAxInteractionTargetActor* SpawnedActor);

	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY()
	TSubclassOf<AAxInteractionTargetActor> TargetClass;

	/** The TargetActor that we spawned */
	UPROPERTY()
	TObjectPtr<AAxInteractionTargetActor> TargetActor;
	
};
