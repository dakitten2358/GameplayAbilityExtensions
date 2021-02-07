#include "AxBlueprintFunctionLibrary.h"
#include "AxGameplayEffectContext.h"
#include "AxGameplayEffectContainer.h"

FGameplayAbilityTargetDataHandle UAxBlueprintFunctionLibrary::EffectContextGetTargetData(FGameplayEffectContextHandle EffectContextHandle)
{
	FAxGameplayEffectContext* AxEffectContext = (FAxGameplayEffectContext*)EffectContextHandle.Get();
	if (AxEffectContext)
	{
		return AxEffectContext->GetTargetData();
	}

	return FGameplayAbilityTargetDataHandle();
}

void UAxBlueprintFunctionLibrary::EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData)
{
	FAxGameplayEffectContext* EffectContext = (FAxGameplayEffectContext*)EffectContextHandle.Get();
	if (EffectContext)
	{
		EffectContext->AddTargetData(TargetData);
	}
}

FGameplayCueParameters UAxBlueprintFunctionLibrary::MakeGameplayCueParametersFromTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 GameplayEffectLevel)
{
	EffectContextAddTargetData(EffectContextHandle, TargetDataHandle);
	FGameplayEffectContext* EffectContext = EffectContextHandle.Get();

	FGameplayCueParameters Parameters;
	Parameters.EffectContext = EffectContextHandle;
	Parameters.SourceObject = EffectContext->GetSourceObject();
	Parameters.Instigator = EffectContext->GetInstigator();
	Parameters.EffectCauser = EffectContext->GetEffectCauser();
	Parameters.AbilityLevel = EffectContext->GetAbilityLevel();
	Parameters.TargetAttachComponent = nullptr;

	if (TargetDataHandle.Num() > 0)
	{
		const FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(0);
		const FHitResult* HitResult = TargetData->GetHitResult();
		if (HitResult)
		{
			Parameters.Location = HitResult->Location;
			Parameters.Normal = HitResult->Normal;
			Parameters.PhysicalMaterial = HitResult->PhysMaterial;
		}		
	}
	
	Parameters.GameplayEffectLevel = GameplayEffectLevel;
	return Parameters;
}

void UAxBlueprintFunctionLibrary::ClearTargetData(FGameplayAbilityTargetDataHandle& TargetData)
{
	TargetData.Clear();
}

UAxGameplayAbility* UAxBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			return Cast<UAxGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

UAxGameplayAbility* UAxBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass);
		if (AbilitySpec)
		{
			return Cast<UAxGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

bool UAxBlueprintFunctionLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}

void UAxBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(FAxGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	ContainerSpec.AddTargets(TargetData, HitResults, TargetActors);
}

void UAxBlueprintFunctionLibrary::AddTargetActorToEffectContainerSpec(FAxGameplayEffectContainerSpec& ContainerSpec, AActor* TargetActor)
{
	ContainerSpec.AddTargetActor(TargetActor);
}

void UAxBlueprintFunctionLibrary::AddTargetHitResultToEffectContainerSpec(FAxGameplayEffectContainerSpec& ContainerSpec, const FHitResult& HitResult)
{
	ContainerSpec.AddTargetHitResult(HitResult);
}

TArray<FActiveGameplayEffectHandle> UAxBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(const FAxGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of gameplay effects
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		if (SpecHandle.IsValid())
		{
			// If effect is valid, iterate list of targets and apply to all
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}
	return AllEffects;
}
