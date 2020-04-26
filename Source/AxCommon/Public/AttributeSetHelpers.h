#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

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