#include "AttributeSetInitializer.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PostInitializeAttributesInterface.h"
#include "AxCommonDebug.h"

void FAttributeSetInitializer::InitializeAttributes(UAbilitySystemComponent* AbilitySystemComponent, const UCurveTable* CurveTable, int32 AtLevel)
{
	if (CurveTable == nullptr)
		return;

	TArray<UAttributeSet*> AttributeSets;
	for (UAttributeSet* Set : AbilitySystemComponent->GetSpawnedAttributes())
	{
		AttributeSets.Add(Set);
	}

	UGameplayEffect* InitializeEffect = NewObject<UGameplayEffect>();
	InitializeEffect->DurationPolicy = EGameplayEffectDurationType::Instant;

	for (const TPair<FName, FRealCurve*>& CurveRow : CurveTable->GetRowMap())
	{
		FString RowName = CurveRow.Key.ToString();
		FString SetName;
		FString AttributeName;
		RowName.Split(TEXT("."), &SetName, &AttributeName);

		if (!ensure(!SetName.IsEmpty() && !AttributeName.IsEmpty()))
		{
			UE_LOG(LogAxCommon, Warning, TEXT("FAttributeSetInitializer::InitializeAttributes Unable to parse row %s in %s"), *RowName, *CurveTable->GetName());
			continue;
		}

		UAttributeSet* Set = FindBestAttributeSet(AttributeSets, SetName);
		if (!Set)
		{
			UE_LOG(LogAxCommon, Warning, TEXT("FAttributeSetInitializer::InitializeAttributes Unable to match AttributeSet from %s (row: %s)"), *SetName, *RowName);
			continue;
		}

		FProperty* Property = FindBestAttribute(Set, AttributeName);
		if (!Property)
		{
			UE_LOG(LogAxCommon, Warning, TEXT("FAttributeSetInitializer::InitializeAttributes Unable to match Attribute from %s (row: %s)"), *AttributeName, *RowName);
			continue;
		}

		// this is awful, but it'll do for now
		FRealCurve* Curve = CurveRow.Value;
		for (auto KeyIter = Curve->GetKeyHandleIterator(); KeyIter; ++KeyIter)
		{
			const FKeyHandle& KeyHandle = *KeyIter;

			TPair<float, float> LevelValuePair = Curve->GetKeyTimeValuePair(KeyHandle);
			int32 Level = LevelValuePair.Key;
			float Value = LevelValuePair.Value;

			if (Level == AtLevel)
			{
				FGameplayModifierInfo AttributeModifier;
				AttributeModifier.Attribute = FGameplayAttribute(Property);
				AttributeModifier.ModifierOp = EGameplayModOp::Override;
				AttributeModifier.ModifierMagnitude = FScalableFloat(Value);
				InitializeEffect->Modifiers.Add(AttributeModifier);
				break;
			}
		}
	}

	AbilitySystemComponent->ApplyGameplayEffectToSelf(InitializeEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());

	// if they can be post initialized, do so now
	for (UAttributeSet* Set : AttributeSets)
	{
		if (Set->Implements<UPostInitializeAttributesInterface>())
		{
			IPostInitializeAttributesInterface* SetToPostInitialize = Cast<IPostInitializeAttributesInterface>(Set);
			SetToPostInitialize->PostInitialize();
		}
	}
}

UAttributeSet* FAttributeSetInitializer::FindBestAttributeSet(TArray<UAttributeSet*>& AttributeSets, FString PartialName)
{
	for (auto AttributeSet : AttributeSets)
	{
		if (AttributeSet->GetClass()->GetName().Contains(PartialName))
		{
			return AttributeSet;
		}
	}
	return nullptr;
}

FProperty* FAttributeSetInitializer::FindBestAttribute(UAttributeSet* AttributeSet, FString PartialName)
{
	for (TFieldIterator<FProperty> It(AttributeSet->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;
		if (Property->GetName() == PartialName)
		{
			return Property;
		}
	}
	return nullptr;
}
