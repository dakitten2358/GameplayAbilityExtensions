#pragma once

#include "CoreMinimal.h"
#include "AxGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameplayAbility_Interaction.generated.h"

/**
 * 
 */
UCLASS()
class AXINTERACTIONS_API UGameplayAbility_Interaction : public UAxGameplayAbility
{
	GENERATED_BODY()

private:
	FGameplayEventData InteractionEventData;
	
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag InteractionCompleteTag;

public:
	UGameplayAbility_Interaction(const FObjectInitializer& ObjectInitializer = FObjectInitializer());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
