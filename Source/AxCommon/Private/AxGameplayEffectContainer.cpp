#include "AxGameplayEffectContainer.h"

bool FAxGameplayEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

bool FAxGameplayEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

void FAxGameplayEffectContainerSpec::AddTargets(const TArray<FGameplayAbilityTargetDataHandle>& InTargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for (const FGameplayAbilityTargetDataHandle& TD : InTargetData)
	{
		TargetData.Append(TD);
	}

	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}
}

void FAxGameplayEffectContainerSpec::AddTargetData(const FGameplayAbilityTargetDataHandle& InTargetData)
{
	TargetData.Append(InTargetData);
}

void FAxGameplayEffectContainerSpec::AddTargetActor(const TWeakObjectPtr<AActor>& TargetActor)
{
	if (TargetActor.IsValid())
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Add(TargetActor);
		TargetData.Add(NewData);	
	}
}

void FAxGameplayEffectContainerSpec::AddTargetHitResult(const FHitResult& HitResult)
{
	FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
	TargetData.Add(NewData);
}

void FAxGameplayEffectContainerSpec::ClearTargets()
{
	TargetData.Clear();
}

void FAxGameplayEffectContainerSpec::SetSetByCallerMagnitude(FGameplayTag DataTag, float Magnitude)
{
	if (SetByCallerTagMagnitudes.Contains(DataTag))
		SetByCallerTagMagnitudes[DataTag] = Magnitude;
	else
		SetByCallerTagMagnitudes.Add(DataTag, Magnitude);
}
