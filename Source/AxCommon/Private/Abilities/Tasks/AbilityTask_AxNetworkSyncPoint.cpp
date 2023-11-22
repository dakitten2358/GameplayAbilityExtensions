// (C) 2021 Jay Murray
#include "Abilities/Tasks/AbilityTask_AxNetworkSyncPoint.h"
#include "AbilitySystemComponent.h"

UAbilityTask_AxNetworkSyncPoint::UAbilityTask_AxNetworkSyncPoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicatedEventToListenFor = EAbilityGenericReplicatedEvent::MAX;
}

void UAbilityTask_AxNetworkSyncPoint::OnSignalCallback()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(ReplicatedEventToListenFor, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}
	SyncFinished();
}

UAbilityTask_AxNetworkSyncPoint* UAbilityTask_AxNetworkSyncPoint::AxWaitNetSync(class UGameplayAbility* OwningAbility, EAbilityTaskNetSyncType InSyncType)
{
	UAbilityTask_AxNetworkSyncPoint* MyObj = NewAbilityTask<UAbilityTask_AxNetworkSyncPoint>(OwningAbility);
	MyObj->SyncType = InSyncType;
	return MyObj;
}

void UAbilityTask_AxNetworkSyncPoint::Activate()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get(), IsPredictingClient());

	if (AbilitySystemComponent.IsValid())
	{
		if (IsPredictingClient())
		{
			if (SyncType != EAbilityTaskNetSyncType::OnlyServerWait )
			{
				// As long as we are waiting (!= OnlyServerWait), listen for the GenericSignalFromServer event
				ReplicatedEventToListenFor = EAbilityGenericReplicatedEvent::GenericSignalFromServer;
			}
			if (SyncType != EAbilityTaskNetSyncType::OnlyClientWait)
			{
				// As long as the server is waiting (!= OnlyClientWait), send the Server and RPC for this signal
				AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::GenericSignalFromClient, GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
			}
			
		}
		else if (IsForRemoteClient())
		{
			if (SyncType != EAbilityTaskNetSyncType::OnlyClientWait )
			{
				// As long as we are waiting (!= OnlyClientWait), listen for the GenericSignalFromClient event
				ReplicatedEventToListenFor = EAbilityGenericReplicatedEvent::GenericSignalFromClient;
			}
			if (SyncType != EAbilityTaskNetSyncType::OnlyServerWait)
			{
				// As long as the client is waiting (!= OnlyServerWait), send the Server and RPC for this signal
				AbilitySystemComponent->ClientSetReplicatedEvent(EAbilityGenericReplicatedEvent::GenericSignalFromServer, GetAbilitySpecHandle(), GetActivationPredictionKey());
			}
		}

		if (ReplicatedEventToListenFor != EAbilityGenericReplicatedEvent::MAX)
		{
			CallOrAddReplicatedDelegate(ReplicatedEventToListenFor, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &UAbilityTask_AxNetworkSyncPoint::OnSignalCallback));
		}
		else
		{
			// We aren't waiting for a replicated event, so the sync is complete.
			SyncFinished();
		}
	}
}

void UAbilityTask_AxNetworkSyncPoint::SyncFinished()
{
	if (IsValid(this))
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnSync.ExecuteIfBound();
		}
		EndTask();
	}
}
