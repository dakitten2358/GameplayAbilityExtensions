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
