#include "AxGameplayEffectContext.h"

FGameplayAbilityTargetDataHandle FAxGameplayEffectContext::GetTargetData() const
{
	return TargetData;
}

void FAxGameplayEffectContext::AddTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	TargetData.Append(TargetDataHandle);
}

bool FAxGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	return Super::NetSerialize(Ar, Map, bOutSuccess) && TargetData.NetSerialize(Ar, Map, bOutSuccess);
}

UScriptStruct* FAxGameplayEffectContext::GetScriptStruct() const
{
	return FAxGameplayEffectContext::StaticStruct();
}

FAxGameplayEffectContext* FAxGameplayEffectContext::Duplicate() const
{
	FAxGameplayEffectContext* NewContext = new FAxGameplayEffectContext();
	*NewContext = *this;
	NewContext->AddActors(Actors);
	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	// Shallow copy of TargetData, is this okay?
	NewContext->TargetData.Append(TargetData);
	return NewContext;
}
