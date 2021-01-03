// Fill out your copyright notice in the Description page of Project Settings.


#include "AxGameplayAbilitySet.h"
#include "AxGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AxCommonDebug.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"

void UAxGameplayAbilitySet::Give(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogAxCommon, Error, TEXT("Tried to give AxGameplayAbilitySet to a null AbilitySystemComponent"));
		return;
	}

	for (const TSubclassOf<UGameplayAbility>& Ability : Abilities)
	{
		int32 inputID = INDEX_NONE;
		UAxGameplayAbility* AxAbility = Cast<UAxGameplayAbility>(Ability.GetDefaultObject());
		if (AxAbility)
		{
			inputID = static_cast<int32>(AxAbility->GetAbilityInputID());
		}

		FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, Level, inputID, SourceObject));
		OutAbilitySetHandles.AbilityHandles.Add(AbilitySpecHandle);
	}

	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : Effects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		if (SourceObject) EffectContext.AddSourceObject(SourceObject);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);

			// we only keep track of EGameplayEffectDurationType::Infinite effects, we'll assume ::Instant and ::HasDuration
			// we'll sort themselves out
			if (GameplayEffect.GetDefaultObject()->DurationPolicy == EGameplayEffectDurationType::Infinite)
				OutAbilitySetHandles.EffectHandles.Add(ActiveGEHandle);
		}
	}
}

void UAxGameplayAbilitySet::Give(AActor* Actor, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const
{
	Give(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), SourceObject, Level, OutAbilitySetHandles);
}

void UAxGameplayAbilitySet::Give(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level) const
{
	FAxGameplayAbilitySetHandles DiscardHandles;
	Give(AbilitySystemComponent, SourceObject, Level, DiscardHandles);
}

void UAxGameplayAbilitySet::Give(AActor* Actor, UObject* SourceObject, int32 Level) const
{
	Give(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), SourceObject, Level);
}

void UAxGameplayAbilitySet::Remove(UAbilitySystemComponent* AbilitySystemComponent, FAxGameplayAbilitySetHandles& AbilitySetHandles) const
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogAxCommon, Error, TEXT("Tried to remove AxGameplayAbilitySet from a null AbilitySystemComponent"));
		return;
	}

	if (AbilitySetHandles.EffectHandles.Num() > 0)
	{
		// we'll only remove the abilities that are actually removed
		TArray<FActiveGameplayEffectHandle>& EffectHandles = AbilitySetHandles.EffectHandles;
		for (int EffectIndex = EffectHandles.Num() - 1; EffectIndex >= 0; --EffectIndex)
		{
			const FActiveGameplayEffectHandle& EffectHandle = EffectHandles[EffectIndex];
			if (AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle))
			{
				EffectHandles.RemoveAt(EffectIndex);
			}
		}
	}

	if (AbilitySetHandles.AbilityHandles.Num() > 0)
	{
		// there's no way for us to know if it was actually removed, so we'll empty them all,
		// and then just empty the list
		for (const FGameplayAbilitySpecHandle& AbilityHandle : AbilitySetHandles.AbilityHandles)
		{
			AbilitySystemComponent->ClearAbility(AbilityHandle);
		}
		AbilitySetHandles.AbilityHandles.Empty();
	}
}

void UAxGameplayAbilitySet::Remove(AActor* Actor, FAxGameplayAbilitySetHandles& AbilitySetHandles) const
{
	Remove(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), AbilitySetHandles);
}
