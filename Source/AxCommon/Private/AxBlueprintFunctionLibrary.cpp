#include "AxBlueprintFunctionLibrary.h"
#include "AxGameplayEffectContext.h"
#include "AxGameplayEffectContainer.h"
#include "AxCommonDebug.h"

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

TArray<FActiveGameplayEffectHandle> UAxBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(const FAxGameplayEffectContainerSpec& ContainerSpec, AActor* ExternalInstigator, AActor* ExternalEffectCauser)
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
				FGameplayEffectSpec* EffectSpec = SpecHandle.Data.Get();
				FGameplayEffectContextHandle EffectContextHandle = EffectSpec->GetEffectContext();
				FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
				if (ExternalInstigator || ExternalEffectCauser)
				{
					EffectContext->AddInstigator(ExternalInstigator, ExternalEffectCauser);
				}
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*EffectSpec));
			}
		}
	}
	return AllEffects;
}

FGameplayTargetDataFilterHandle UAxBlueprintFunctionLibrary::MakeAxFilterHandle(FAxGameplayTargetDataFilter Filter, AActor* FilterActor)
{
	FGameplayTargetDataFilterHandle FilterHandle;
	FAxGameplayTargetDataFilter* NewFilter = new FAxGameplayTargetDataFilter(Filter);
	NewFilter->InitializeFilterContext(FilterActor);
	FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);
	return FilterHandle;
}

void UAxBlueprintFunctionLibrary::DrawDebugHitResult(const UObject* WorldContextObject, const FHitResult& HitResult)
{
#if ENABLE_DRAW_DEBUG
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FVector LineStart = HitResult.TraceStart;
		FVector LineEnd = HitResult.Location;

		FColor TraceColor = FColor::Red;
		bool bPersistent = false;
		float LifeTime = 0.5f;
		float Thickness = 2.0f;

		::DrawDebugLine(World, LineStart, LineEnd, TraceColor, bPersistent, LifeTime, SDPG_World, Thickness);

		FVector TraceDirection = (LineEnd - LineStart).GetSafeNormal();
		FVector ReflectionDirection = FMath::GetReflectionVector(TraceDirection, HitResult.Normal);
		FVector ScaledReflectionDirection = ReflectionDirection * 100.0f;
		FVector ReflectionEnd = (LineEnd + ScaledReflectionDirection);

		FColor ReflectionColor = FColor::Green;

		::DrawDebugLine(World, LineEnd, ReflectionEnd, ReflectionColor, bPersistent, LifeTime, SDPG_World, Thickness);
	}
#endif
}
