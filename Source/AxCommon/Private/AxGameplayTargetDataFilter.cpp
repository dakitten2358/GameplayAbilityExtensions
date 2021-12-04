// (C) 2021 Jay Murray


#include "AxGameplayTargetDataFilter.h"
#include "GameFramework/Actor.h"
#include "AxCommonDebug.h"

bool FAxGameplayTargetDataFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{

	bool bFilterResult = Super::FilterPassesForActor(ActorToBeFiltered);
	if (bFilterResult)
	{
		// have we seen this actor?
		TWeakObjectPtr<const AActor> ActorToBeFilteredPtr(ActorToBeFiltered);
		if (AddedActors.Contains(ActorToBeFilteredPtr))
		{
			return false;
		}

		AddedActors.Add(ActorToBeFilteredPtr);
		return true;
	}
	return bFilterResult;
}

bool FAxGameplayTargetDataTeamFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{
	bool bFilterResult = Super::FilterPassesForActor(ActorToBeFiltered);
	if (bFilterResult)
	{
		// does this actor have a team agent interface
		if (SelfActor->GetClass()->ImplementsInterface(UGenericTeamAgentInterface::StaticClass()))
		{
			ETeamAttitude::Type AttitudeTowards = Cast<IGenericTeamAgentInterface>(SelfActor)->GetTeamAttitudeTowards(*ActorToBeFiltered);
			return (bReverseTeamAttitude ^ (TeamAttitude == AttitudeTowards));
		}

		UE_LOG(LogAxCommon, Warning, TEXT("Tried to filter for %s that didn't support IGenericTeamAgentInterface"), *AActor::GetDebugName(ActorToBeFiltered));
		return false;
	}
	return bFilterResult;
}

bool FAxGameplayTargetDataInterfaceFilter::FilterPassesForActor(const AActor* ActorToBeFiltered) const
{
	bool bFilterResult = Super::FilterPassesForActor(ActorToBeFiltered);
	if (bFilterResult && ActorToBeFiltered && ActorToBeFiltered->GetClass()->ImplementsInterface(ImplementsInterface))
		return true;
	return bFilterResult;
}