// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorators/BTDecorator_HasGameplayTag.h"
#include "AbilitySystemComponent.h"

UBTDecorator_HasGameplayTag::UBTDecorator_HasGameplayTag(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("HasGameplayTags");
	bNotifyProcessed = true;

	bAllowAbortNone = false;
	bAllowAbortLowerPri = false;
	bAllowAbortChildNodes = false;
}

bool UBTDecorator_HasGameplayTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, UAbilitySystemComponent* AbilitySystemComponent, uint8* NodeMemory) const
{
	if (AbilitySystemComponent->HasAllMatchingGameplayTags(Tags))
		return true;
	return false;
}

FString UBTDecorator_HasGameplayTag::GetStaticDescription() const
{
	FString CollectedTagStrings;
	for (int i = 0; i < Tags.Num(); ++i)
	{
		if (i != 0)
			CollectedTagStrings.Append(TEXT(", "));
		CollectedTagStrings.Append(Tags.GetByIndex(i).ToString());
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *CollectedTagStrings);
}