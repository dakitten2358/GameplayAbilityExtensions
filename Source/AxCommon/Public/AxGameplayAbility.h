// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputAction.h"
#include "AxGameplayEffectContainer.h" 
#include "AxGameplayAbility.generated.h"

/**
 *
 */
UCLASS(Abstract)
class AXCOMMON_API UAxGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual bool TryActivateAbilityBatched(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ExternalEndAbility();
public:
	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilites forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bPreferEventActivation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input", meta=(DisplayName="Active on InputAction"))
	bool bActivateAbilityOnInputAction = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input")
	TObjectPtr<const UInputAction> AbilityInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TMap<FGameplayTag, FAxGameplayEffectContainer> EffectContainerMap;

	/** Retrieves actor's name used for logging, or string "NULL" if Actor is null */
	static FString GetDebugName(const UGameplayAbility* Ability);

protected:
	// Make gameplay effect container spec to be applied later, using the passed in container
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FAxGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FAxGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	// Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FAxGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);

	// Applies a gameplay effect container spec that was previously created
	UFUNCTION(BlueprintCallable, Category = "Ability")
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FAxGameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category = "Target")
	virtual FGameplayAbilityTargetDataHandle MakeTargetDataFromActor(AActor* TargetActor, bool bShouldReplicateDataToServer = true, bool bCreateKeyIfNotValidForMorePredicting = true);
};
