// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SendGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AxCommonDebug.h"

FString UAnimNotify_SendGameplayEvent::GetNotifyName_Implementation() const
{
	return EventTag.GetTagName().ToString();
}

void UAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	AActor* Actor = MeshComp->GetOwner();
	if (IsValid(Actor))
	{
		IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Actor);
		if (AbilitySystemInterface != nullptr)
		{
			UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
			if (IsValid(AbilitySystemComponent))
			{
				FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);

				Payload.EventTag = EventTag;
				Payload.EventMagnitude = EventMagnitude;

				AbilitySystemComponent->HandleGameplayEvent(EventTag, &Payload);
			}
			else
			{
				UE_LOG(LogAxCommon, Error, TEXT("UAnimNotify_SendGameplayEvent:  Invalid ability system component retrieved from Actor %s. EventTag was %s"), *Actor->GetName(), *EventTag.ToString());
			}
		}
	}
}