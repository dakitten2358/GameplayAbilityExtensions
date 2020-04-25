// Fill out your copyright notice in the Description page of Project Settings.


#include "AxGameInstanceSubsystem.h"
#include "AbilitySystemGlobals.h"

void UAxGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UAbilitySystemGlobals::Get().InitGlobalData();
}