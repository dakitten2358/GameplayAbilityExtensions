// Straight up copied from GASShooter
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayEffectTypes.h"
#include "AxGameplayEffectContext.generated.h"

USTRUCT()
struct AXCOMMON_API FAxGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()

public:
	FGameplayAbilityTargetDataHandle GetTargetData() const;
	void AddTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	// FGameplayEffectContext
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FAxGameplayEffectContext* Duplicate() const override;
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	FGameplayAbilityTargetDataHandle TargetData;
};

template<>
struct TStructOpsTypeTraits<FAxGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAxGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

