// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "AxGameplayTargetDataFilter.generated.h"

/**
 * 
 */
USTRUCT()
struct AXCOMMON_API FAxGameplayTargetDataFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

public:

	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;

private:
	mutable TArray<TWeakObjectPtr<const class AActor>> AddedActors;
};
