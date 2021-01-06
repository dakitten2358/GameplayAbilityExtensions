#include "AxGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AxAbilitySystemComponent.h"
#include "AxCommonDebug.h"
#include "GameFramework/Actor.h"

void UAxGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
		if (!ActivatedAbility)
		{
			UE_LOG(LogAxCommon, Warning, TEXT("Failed to auto-activated ability on %s"), *ActorInfo->OwnerActor->GetName());
		}
	}
}

void UAxGameplayAbility::ExternalEndAbility()
{
	check(CurrentActorInfo);

	const bool bReplicateEndAbility = true;
	const bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UAxGameplayAbility::TryActivateAbilityBatched(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	UAxAbilitySystemComponent* AbilitySystem = Cast<UAxAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (AbilitySystem)
	{
		return AbilitySystem->TryActivateAbilityBatched(InAbilityHandle, EndAbilityImmediately);
	}

	return false;
}
