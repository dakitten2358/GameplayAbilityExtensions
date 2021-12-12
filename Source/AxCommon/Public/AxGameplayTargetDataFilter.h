// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "GenericTeamAgentInterface.h"
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

USTRUCT()
struct AXCOMMON_API FAxGameplayTargetDataTeamFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

public:
	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
   	TEnumAsByte<ETeamAttitude::Type> TeamAttitude = ETeamAttitude::Hostile;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	bool bReverseTeamAttitude = false;
};

USTRUCT()
struct AXCOMMON_API FAxGameplayTargetDataInterfaceFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

public:
	FAxGameplayTargetDataInterfaceFilter();
	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	TObjectPtr<UClass> ImplementsInterface;
};
