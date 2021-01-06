// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "InputAction.h"
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

	void ExternalEndAbility();
public:
	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilites forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bActivateAbilityOnGranted;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input", meta=(DisplayName="Active on InputAction"))
	bool bActivateAbilityOnInputAction = true;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Input")
	const UInputAction* AbilityInputAction;
};
