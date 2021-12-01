#include "AxTask_WaitInteractionTargetData.h"
#include "AbilitySystemComponent.h"
#include "AxInteractionInterface.h"
#include "AxInteractionTargetActor.h"

UAxTask_WaitInteractionTargetData* UAxTask_WaitInteractionTargetData::WaitForInteractionTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<AAxInteractionTargetActor> InTargetClass, float TimerPeriod, bool bShowDebug)
{
	UAxTask_WaitInteractionTargetData* MyObj = NewAbilityTask<UAxTask_WaitInteractionTargetData>(OwningAbility, TaskInstanceName);
	MyObj->TargetClass = InTargetClass;
	MyObj->TargetActor = nullptr;
	MyObj->TimerPeriod = TimerPeriod;
	return MyObj;
}

UAxTask_WaitInteractionTargetData* UAxTask_WaitInteractionTargetData::WaitForInteractionTargetDataWithActor(UGameplayAbility* OwningAbility, FName TaskInstanceName, AAxInteractionTargetActor* InteractionTargetActor, float TimerPeriod, bool bShowDebug)
{
	UAxTask_WaitInteractionTargetData* MyObj = NewAbilityTask<UAxTask_WaitInteractionTargetData>(OwningAbility, TaskInstanceName);
	MyObj->TargetClass = InteractionTargetActor->GetClass();
	MyObj->TargetActor = InteractionTargetActor;
	MyObj->TimerPeriod = TimerPeriod;
	MyObj->InitializeTargetActor(MyObj->TargetActor);
	return MyObj;
}

void UAxTask_WaitInteractionTargetData::Activate()
{
	Super::Activate();

	check(TargetClass != nullptr);

	const bool bLoop = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAxTask_WaitInteractionTargetData::UpdateInteractionTarget, TimerPeriod, bLoop);
}

void UAxTask_WaitInteractionTargetData::OnDestroy(bool bAbilityEnded)
{
	if (TimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnDestroy(bAbilityEnded);
}

void UAxTask_WaitInteractionTargetData::UpdateInteractionTarget()
{
	if (TargetActor)
	{
		FHitResult TargetHitResult;
		TargetActor->CheckForInteractionTarget(TargetHitResult);

		// filter it to usable interaction items
		if (TargetHitResult.bBlockingHit)
		{
			if (TargetHitResult.GetActor() && TargetHitResult.GetActor()->Implements<UAxInteractionInterface>())
			{
				// it does implement the interface, but can we currently interact with it?
				TargetHitResult.bBlockingHit = IAxInteractionInterface::Execute_IsInteractionTarget(HitActor, GetOwnerActor());
			}
			else
			{
				// no actor, or it doesn't even implement the
				// required interface
				TargetHitResult.bBlockingHit = false;
			}

			// clean up the hit result if it changed
			if (!TargetHitResult.bBlockingHit)
			{
				//TODO(jm): removed actor reset -> check to see if we use that anyone (almost certainly...)
				TargetHitResult.Component.Reset();
			}
		}
		
		// we didn't hit anything
		if (!TargetHitResult.bBlockingHit)
		{
			// do we have something already?
			if (TargetData.Num() > 0 && TargetData.Get(0)->GetHitResult()->GetActor())
			{
				// Previous trace had a valid Interactable Actor, now we don't have one
				// Broadcast last valid target
				LostInteractionTarget.Broadcast(TargetData);
			}

			TargetData = TargetActor->MakeTargetData(TargetHitResult);
		}
		// we did hit something
		else
		{
			bool bBroadcastNewTarget = true;

			if (TargetData.Num() > 0)
			{
				const AActor* OldTarget = TargetData.Get(0)->GetHitResult()->GetActor();

				if (OldTarget == TargetHitResult.GetActor())
				{
					// Old target is the same as the new target, don't broadcast the target
					bBroadcastNewTarget = false;
				}
				else if (OldTarget)
				{
					// Old target exists and is different from the new target
					// Broadcast last valid target
					LostInteractionTarget.Broadcast(TargetData);
				}
			}

			if (bBroadcastNewTarget)
			{
				// Broadcast new valid target
				TargetData = TargetActor->MakeTargetData(TargetHitResult);
				FoundNewInteractionTarget.Broadcast(TargetData);
			}
		}		
	}
}

bool UAxTask_WaitInteractionTargetData::BeginSpawningActor(UGameplayAbility* OwningAbility, TSubclassOf<AAxInteractionTargetActor> InTargetClass, AAxInteractionTargetActor*& SpawnedActor)
{
	SpawnedActor = nullptr;

	if (Ability)
	{
		UClass* Class = *InTargetClass;
		if (Class != nullptr)
		{
			if (UWorld* World = GEngine->GetWorldFromContextObject(OwningAbility, EGetWorldErrorMode::LogAndReturnNull))
			{
				SpawnedActor = World->SpawnActorDeferred<AAxInteractionTargetActor>(Class, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			}
		}

		if (SpawnedActor)
		{
			TargetActor = SpawnedActor;
			InitializeTargetActor(SpawnedActor);
		}
	}
	return (SpawnedActor != nullptr);
}

void UAxTask_WaitInteractionTargetData::FinishSpawningActor(UGameplayAbility* OwningAbility, AAxInteractionTargetActor* SpawnedActor)
{
	if (IsValid(SpawnedActor))
	{
		check(TargetActor == SpawnedActor);

		const FTransform SpawnTransform = AbilitySystemComponent->GetOwner()->GetTransform();

		SpawnedActor->FinishSpawning(SpawnTransform);

		FinalizeTargetActor(SpawnedActor);
	}
}

void UAxTask_WaitInteractionTargetData::InitializeTargetActor(AAxInteractionTargetActor* SpawnedActor)
{
	SpawnedActor->MasterPC = Ability->GetCurrentActorInfo()->PlayerController.Get();
}

void UAxTask_WaitInteractionTargetData::FinalizeTargetActor(AAxInteractionTargetActor* SpawnedActor)
{
	SpawnedActor->StartTargeting(Ability);
}
