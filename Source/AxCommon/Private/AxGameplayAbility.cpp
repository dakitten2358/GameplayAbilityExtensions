#include "AxGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AxAbilitySystemComponent.h"
#include "AxCommonDebug.h"
#include "GameFramework/Actor.h"
#include "AxGameplayTargetProvider.h"

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

FAxGameplayEffectContainerSpec UAxGameplayAbility::MakeEffectContainerSpecFromContainer(const FAxGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FAxGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	UAbilitySystemComponent* OwningASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);
	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetProvider.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			TArray<FGameplayAbilityTargetDataHandle> TargetData;
			const UAxGameplayTargetProvider* TargetProviderCDO = Container.TargetProvider.GetDefaultObject();
			TargetProviderCDO->GetTargets(AvatarActor, EventData, TargetData, HitResults, TargetActors);
			ReturnSpec.AddTargets(TargetData, HitResults, TargetActors);
 		}

		// If we don't have an override level, use the ability level
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = GetAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FAxGameplayEffectContainerSpec UAxGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FAxGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FAxGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UAxGameplayAbility::ApplyEffectContainerSpec(const FAxGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}


FGameplayAbilityTargetDataHandle UAxGameplayAbility::MakeTargetDataFromActor(AActor* TargetActor)
{
	TArray<TWeakObjectPtr<AActor>> TargetActors;
	TargetActors.Add(TargetActor);

	FGameplayAbilityTargetingLocationInfo targetingLocationInfo;
	FGameplayAbilityTargetDataHandle TargetDataHandle = targetingLocationInfo.MakeTargetDataHandleFromActors(TargetActors);

	UAbilitySystemComponent* OwningASC = GetAbilitySystemComponentFromActorInfo();
	if (OwningASC)	
	{
		FGameplayTag ApplicationTag;
		OwningASC->ServerSetReplicatedTargetData(GetCurrentAbilitySpecHandle(), GetCurrentActivationInfo().GetActivationPredictionKey(), TargetDataHandle, ApplicationTag, OwningASC->ScopedPredictionKey);
	}

	return TargetDataHandle;
}
