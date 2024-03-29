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

	FAxGameplayAbilitySetData::GiveAbilities(AbilitySystemComponent, SourceObject, Level, bRemoveAbilitiesAfterActivation, Abilities, OutAbilitySetHandles);
	FAxGameplayAbilitySetData::GiveEffects(AbilitySystemComponent, SourceObject, Level, Effects, OutAbilitySetHandles);
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

	FAxGameplayAbilitySetData::RemoveEffects(AbilitySystemComponent, AbilitySetHandles);
	FAxGameplayAbilitySetData::RemoveAbilities(AbilitySystemComponent, AbilitySetHandles);
}

void UAxGameplayAbilitySet::Remove(AActor* Actor, FAxGameplayAbilitySetHandles& AbilitySetHandles) const
{
	Remove(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), AbilitySetHandles);
}

void FAxGameplayAbilitySetData::Give(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const
{
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogAxCommon, Error, TEXT("Tried to give FAxGameplayAbilitySetData to a null AbilitySystemComponent"));
		return;
	}

	const bool bRemoveAbilitiesAfterActivation = false;
	FAxGameplayAbilitySetData::GiveAbilities(AbilitySystemComponent, SourceObject, Level, bRemoveAbilitiesAfterActivation, Abilities, OutAbilitySetHandles);
	FAxGameplayAbilitySetData::GiveEffects(AbilitySystemComponent, SourceObject, Level, Effects, OutAbilitySetHandles);
}

void FAxGameplayAbilitySetData::Give(AActor* Actor, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const
{
	Give(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), SourceObject, Level, OutAbilitySetHandles);
}

void FAxGameplayAbilitySetData::Remove(UAbilitySystemComponent* AbilitySystemComponent, FAxGameplayAbilitySetHandles& AbilitySetHandles) const
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogAxCommon, Error, TEXT("Tried to remove FAxGameplayAbilitySetData from a null AbilitySystemComponent"));
		return;
	}

	FAxGameplayAbilitySetData::RemoveEffects(AbilitySystemComponent, AbilitySetHandles);
	FAxGameplayAbilitySetData::RemoveAbilities(AbilitySystemComponent, AbilitySetHandles);
}

void FAxGameplayAbilitySetData::Remove(AActor* Actor, FAxGameplayAbilitySetHandles& AbilitySetHandles) const
{
	Remove(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor), AbilitySetHandles);
}

bool FAxGameplayAbilitySetData::Any() const
{
	return Abilities.Num() > 0 || Effects.Num() > 0;
}

void FAxGameplayAbilitySetData::GiveAbilities(class UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level, bool bRemoveAbilitiesAfterActivation, const TArray<TSubclassOf<class UGameplayAbility>>& Abilities, FAxGameplayAbilitySetHandles& OutAbilitySetHandles)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : Abilities)
	{
		if (!AbilityClass)
		{
			UE_LOG(LogAxCommon, Error, TEXT("Tried to give a null ability while granting abilities to %s"), *AActor::GetDebugName(AbilitySystemComponent->GetOwner()));
			continue;
		}
		int32 inputID = INDEX_NONE;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level, inputID, SourceObject);
		AbilitySpec.RemoveAfterActivation = bRemoveAbilitiesAfterActivation;
		FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
		OutAbilitySetHandles.AbilityHandles.Add(AbilitySpecHandle);
	}
}

void FAxGameplayAbilitySetData::GiveEffects(class UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level, const TArray<TSubclassOf<class UGameplayEffect>>& Effects, FAxGameplayAbilitySetHandles& OutAbilitySetHandles)
{
	for (const TSubclassOf<UGameplayEffect>& GameplayEffectClass : Effects)
	{
		if (!GameplayEffectClass)
		{
			UE_LOG(LogAxCommon, Error, TEXT("Tried to give a null ability while granting abilities to %s"), *AActor::GetDebugName(AbilitySystemComponent->GetOwner()));
			continue;
		}

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		if (SourceObject) EffectContext.AddSourceObject(SourceObject);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);

			// we only keep track of EGameplayEffectDurationType::Infinite effects, we'll assume ::Instant and ::HasDuration
			// we'll sort themselves out
			if (GameplayEffectClass.GetDefaultObject()->DurationPolicy == EGameplayEffectDurationType::Infinite)
				OutAbilitySetHandles.EffectHandles.Add(ActiveGEHandle);
		}
	}
}

void FAxGameplayAbilitySetData::RemoveAbilities(class UAbilitySystemComponent* AbilitySystemComponent, FAxGameplayAbilitySetHandles& AbilitySetHandles)
{
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

void FAxGameplayAbilitySetData::RemoveEffects(class UAbilitySystemComponent* AbilitySystemComponent, FAxGameplayAbilitySetHandles& AbilitySetHandles)
{
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
}
