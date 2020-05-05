#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

struct AXCOMMON_API FAttributeSetInitializer
{
public:
	static void InitializeAttributes(class UAbilitySystemComponent* AbilitySystemComponent, const class UCurveTable* CurveTable, int32 Level);

private:
	static UAttributeSet* FindBestAttributeSet(TArray<UAttributeSet*>& AttributeSets, FString PartialName);
	static FProperty* FindBestAttribute(UAttributeSet* AttributeSet, FString PartialName);
};
