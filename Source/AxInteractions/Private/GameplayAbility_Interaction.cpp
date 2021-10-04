#include "GameplayAbility_Interaction.h"

UGameplayAbility_Interaction::UGameplayAbility_Interaction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InteractionCompleteTag = FGameplayTag::RequestGameplayTag("Ability.Interaction.Complete");
}

void UGameplayAbility_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData)
		InteractionEventData = *TriggerEventData;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// no matter what, we need send an event so the passive ability works
	SendGameplayEvent(InteractionCompleteTag, InteractionEventData);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
