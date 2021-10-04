#include "GameplayAbility_InteractPassive.h"
#include "AxTask_WaitInteractionTargetData.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AxInteractioninterface.h"
#include "AxInteractionTargetActor_LineTrace.h"

const FName UGameplayAbility_InteractPassive::WaitInteractionTargetDataTaskName("WaitInteractionTargetData");

UGameplayAbility_InteractPassive::UGameplayAbility_InteractPassive(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InteractionCompleteTag = FGameplayTag::RequestGameplayTag("Ability.Interaction.Complete");
	InteractionTargetActorClass = AAxInteractionTargetActor_LineTrace::StaticClass();
	bActivateAbilityOnGranted = true;
	bActivateAbilityOnInputAction = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_InteractPassive::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const float TimerPeriod = 0.1f;
	const bool bDebug = true;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	InteractionTargetActor = World->SpawnActorDeferred<AAxInteractionTargetActor>(InteractionTargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	InteractionTargetActor->StartLocation = MakeTargetLocationInfoFromOwnerActor();
	const FTransform SpawnTransform = GetAbilitySystemComponentFromActorInfo()->GetOwner()->GetTransform();
	InteractionTargetActor->FinishSpawning(SpawnTransform);
	InteractionTargetActor->StartTargeting(this);

	WaitInteractionTargetDataTask = UAxTask_WaitInteractionTargetData::WaitForInteractionTargetDataWithActor(this, WaitInteractionTargetDataTaskName, InteractionTargetActor, TimerPeriod, bDebug);
	WaitInteractionTargetDataTask->FoundNewInteractionTarget.AddUniqueDynamic(this, &Self::OnFoundNewInteractionTarget);
	WaitInteractionTargetDataTask->LostInteractionTarget.AddUniqueDynamic(this, &Self::OnLostInteractionTarget);
	WaitInteractionTargetDataTask->ReadyForActivation();

	WaitInteractionCompleteEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, InteractionCompleteTag);
	WaitInteractionCompleteEventTask->OnlyMatchExact = true;
	WaitInteractionCompleteEventTask->EventReceived.AddUniqueDynamic(this, &Self::OnInteractionComplete);
	WaitInteractionCompleteEventTask->ReadyForActivation();
}

void UGameplayAbility_InteractPassive::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (IsValid(InteractionTargetActor))
		InteractionTargetActor->Destroy();
}

void UGameplayAbility_InteractPassive::OnFoundNewInteractionTarget(const FGameplayAbilityTargetDataHandle& Data)
{
	SetNewInteractionTarget(Data);
	K2_OnFoundNewInteractionTarget(Data);
	StartWaitForInteractionInputPressed();
}

void UGameplayAbility_InteractPassive::OnLostInteractionTarget(const FGameplayAbilityTargetDataHandle& Data)
{
	if (IsValid(WaitInteractionInputPressed))
	{
		WaitInteractionInputPressed->EndTask();
		WaitInteractionInputPressed = nullptr;
	}

	if (IsValid(WaitInteractionInputReleased))
	{
		WaitInteractionInputReleased->EndTask();
		WaitInteractionInputReleased = nullptr;
	}

	K2_OnLostInteractionTarget(Data);
	ClearInteractionTarget();
}

void UGameplayAbility_InteractPassive::OnInteractionComplete(FGameplayEventData Payload)
{
	K2_OnInteractionComplete(Payload);

	AActor* InteractionCompletedForActor = GetInteractionInterface(Payload.TargetData);
	AActor* CurrentInteractionActor = GetInteractionInterface(CurrentInteractionTarget);
	
	// if we've recorded a press, and the target hasn't changed, we need to wait for a release, before waiting for a new press
	if (!IsValid(WaitInteractionInputPressed) && IsValid(InteractionCompletedForActor) && IsValid(CurrentInteractionActor) && CurrentInteractionActor == InteractionCompletedForActor)
	{
		StartWaitForInteractionInputReleased();
	}
}

void UGameplayAbility_InteractPassive::OnInteractionInputPressed(float TimeWaited)
{
	if (IsValid(WaitInteractionInputPressed))
	{
		WaitInteractionInputPressed->EndTask();
		WaitInteractionInputPressed = nullptr;
	}

	AActor* CurrentInteractionTargetActor = GetInteractionInterface(CurrentInteractionTarget);
	FGameplayTag EventTag = IAxInteractionInterface::Execute_GetStartInteractionEvent(CurrentInteractionTargetActor);
	FGameplayEventData Payload;
	Payload.Target = CurrentInteractionTargetActor;
	Payload.TargetData = CurrentInteractionTarget;
	SendGameplayEvent(EventTag, Payload);

	K2_OnInteractionStart(CurrentInteractionTarget);
}

void UGameplayAbility_InteractPassive::OnInteractionInputReleased(float TimeWaited)
{
	if (IsValid(WaitInteractionInputReleased))
	{
		WaitInteractionInputReleased->EndTask();
		WaitInteractionInputReleased = nullptr;
	}

	StartWaitForInteractionInputPressed();
}

void UGameplayAbility_InteractPassive::StartWaitForInteractionInputPressed()
{
	check(WaitInteractionInputPressed == nullptr);

	const bool bTestAlreadyPressed = true;
	WaitInteractionInputPressed = UAbilityTask_WaitInputPress::WaitInputPress(this, bTestAlreadyPressed);
	WaitInteractionInputPressed->OnPress.AddUniqueDynamic(this, &Self::OnInteractionInputPressed);
	WaitInteractionInputPressed->ReadyForActivation();
}

void UGameplayAbility_InteractPassive::StartWaitForInteractionInputReleased()
{
	const bool bTestAlreadyPressed = true;
	WaitInteractionInputReleased = UAbilityTask_WaitInputRelease::WaitInputRelease(this, bTestAlreadyPressed);
	WaitInteractionInputReleased->OnRelease.AddUniqueDynamic(this, &Self::OnInteractionInputReleased);
	WaitInteractionInputReleased->ReadyForActivation();
}

void UGameplayAbility_InteractPassive::SetNewInteractionTarget(const FGameplayAbilityTargetDataHandle& Data)
{
	ClearInteractionTarget();

	if (AActor* NewInteractionTargetActor = GetInteractionInterface(Data))
	{
		CurrentInteractionTarget = Data;
		IAxInteractionInterface::Execute_StartInteractionHighlighting(NewInteractionTargetActor);
	}
}

void UGameplayAbility_InteractPassive::ClearInteractionTarget()
{
	if (AActor* ExistingInteractionTargetActor = GetInteractionInterface(CurrentInteractionTarget))
		IAxInteractionInterface::Execute_StopInteractionHighlighting(ExistingInteractionTargetActor);

	CurrentInteractionTarget.Clear();
}

AActor* UGameplayAbility_InteractPassive::GetInteractionInterface(const FGameplayAbilityTargetDataHandle& Data)
{
	const int Index = 0;
	if (const FGameplayAbilityTargetData* TargetData = Data.Get(Index))
	{
		 for(TWeakObjectPtr<AActor> ActorPtr : TargetData->GetActors())
		 {
			 if (AActor* Actor = ActorPtr.Get())
			 {
				 if (Actor->GetClass()->ImplementsInterface(UAxInteractionInterface::StaticClass()))
				 {
					return Actor;
				 }
			 }
		 }
	}
	return nullptr;
}
