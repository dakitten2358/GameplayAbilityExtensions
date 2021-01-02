#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "GameplayTagContainer.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// For ints
#define GAMEPLAYATTRIBUTE_VALUE_INT_GETTER(PropertyName) \
	FORCEINLINE int32 Get##PropertyName() const \
	{ \
		return static_cast<int32>(PropertyName.GetCurrentValue()); \
	}

#define GAMEPLAYATTRIBUTE_VALUE_INT_SETTER(PropertyName) \
	FORCEINLINE void Set##PropertyName(int32 NewVal) \
	{ \
		UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent(); \
		if (ensure(AbilityComp)) \
		{ \
			AbilityComp->SetNumericAttributeBase(Get##PropertyName##Attribute(), NewVal); \
		}; \
	}

#define GAMEPLAYATTRIBUTE_VALUE_INT_INITTER(PropertyName) \
	FORCEINLINE void Init##PropertyName(int32 NewVal) \
	{ \
		PropertyName.SetBaseValue(NewVal); \
		PropertyName.SetCurrentValue(NewVal); \
	}

#define ATTRIBUTE_INT_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INT_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INT_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INT_INITTER(PropertyName)

template<typename CharacterType>
struct FPostGameplayEffectExecuteData
{
public:
	struct SourceActorInfo
	{
		AActor* Actor;
		AController* Controller;
		CharacterType* Character;

		const FGameplayTagContainer& Tags;

		SourceActorInfo(const FGameplayTagContainer& SourceTags)
			: Actor(nullptr)
			, Controller(nullptr)
			, Character(nullptr)
			, Tags(SourceTags)
		{}
	};

	struct TargetActorInfo
	{
		AActor* Actor;
		AController* Controller;
		CharacterType* Character;

		class UAbilitySystemComponent& AbilitySystem;

		TargetActorInfo(class UAbilitySystemComponent& ASC)
			: Actor(nullptr)
			, Controller(nullptr)
			, Character(nullptr)
			, AbilitySystem(ASC)
		{}
	};

	TargetActorInfo Target;
	SourceActorInfo Source;

	FGameplayTagContainer EffectSpecAssetTags;

	const struct FGameplayEffectSpec& EffectSpec;		// The spec that the mod came from
	struct FGameplayModifierEvaluatedData& EvaluatedData;	// The 'flat'/computed data to be applied to the target

	const struct FHitResult* HitResult;

	FPostGameplayEffectExecuteData<CharacterType>(const struct FGameplayEffectModCallbackData& Data)
		: Source(*Data.EffectSpec.CapturedSourceTags.GetAggregatedTags())
		, Target(Data.Target)
		, EffectSpec(Data.EffectSpec)
		, EvaluatedData(Data.EvaluatedData)
		, HitResult(nullptr)
	{
		FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
		UAbilitySystemComponent* SourceAbility = Context.GetOriginalInstigatorAbilitySystemComponent();
		Data.EffectSpec.GetAllAssetTags(EffectSpecAssetTags);
		HitResult = Context.GetHitResult();		

		// Get the Target actor, which should be our owner
		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			Target.Actor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			Target.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
			Target.Character = Cast<CharacterType>(Target.Actor);
		}

		// Get the Source actor
		if (SourceAbility && SourceAbility->AbilityActorInfo.IsValid() && SourceAbility->AbilityActorInfo->AvatarActor.IsValid())
		{
			Source.Actor = SourceAbility->AbilityActorInfo->AvatarActor.Get();
			Source.Controller = SourceAbility->AbilityActorInfo->PlayerController.Get();
			if (Source.Controller == nullptr && Source.Actor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(Source.Actor))
				{
					Source.Controller = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (Source.Controller)
			{
				Source.Character = Cast<CharacterType>(Source.Controller->GetPawn());
			}
			else
			{
				Source.Character = Cast<CharacterType>(Source.Actor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				Source.Actor = Context.GetEffectCauser();
			}
		}
	}
};
