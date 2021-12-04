// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayAbilitySpec.h"
#include "AxGameplayEffectContainer.h"
#include "AxGameplayTargetDataFilter.h"
#include "AxBlueprintFunctionLibrary.generated.h"

class UAxGameplayAbility;

/**
 * 
 */
UCLASS()
class AXCOMMON_API UAxBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    // Returns TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "GetTargetData"))
	static FGameplayAbilityTargetDataHandle EffectContextGetTargetData(FGameplayEffectContextHandle EffectContext);

	// Adds TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "AddTargetData"))
	static void EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData);

	// Clears a Target Data
	UFUNCTION(BlueprintCallable, Category = "Ability|TargetData")
	static void ClearTargetData(UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetData);

	// Native make, to avoid having to deal with quantized vector types
	UFUNCTION(BlueprintPure, Category = "Ability|GameplayCue", meta = (NativeMakeFunc, AdvancedDisplay=2, GameplayEffectLevel = "1"))
	static FGameplayCueParameters MakeGameplayCueParametersFromTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetDataHandle, int32 GameplayEffectLevel);

	static FGameplayCueParameters MakeGameplayCueParametersFromGameplayEffectData(const struct FGameplayEffectModCallbackData& EffectData);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UAxGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UAxGameplayAbility* GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Ability|Container", Meta = (AutoCreateRefTerm = "TargetData, HitResults, TargetActors"))
	static void AddTargetsToEffectContainerSpec(UPARAM(ref) FAxGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static void AddTargetActorToEffectContainerSpec(UPARAM(ref) FAxGameplayEffectContainerSpec& ContainerSpec, AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static void AddTargetHitResultToEffectContainerSpec(UPARAM(ref) FAxGameplayEffectContainerSpec& ContainerSpec, const FHitResult& HitResult);

	// Applies container spec that was made from an ability
	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FAxGameplayEffectContainerSpec& ContainerSpec, AActor* ExternalInstigator = nullptr, AActor* ExternalEffectCauser = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static void AssignTagSetByCallerMagnitudeContainerSpec(UPARAM(ref) FAxGameplayEffectContainerSpec& ContainerSpec, FGameplayTag DataTag, float Magnitude);

	/** Create a handle for filtering target data, filling out all fields */
	UFUNCTION(BlueprintCallable, Category = "Filter")
	static FGameplayTargetDataFilterHandle MakeAxFilterHandle(FAxGameplayTargetDataFilter Filter, AActor* FilterActor);

	UFUNCTION(BlueprintPure, Category = "Filter")
	static FGameplayTargetDataFilterHandle MakeAxTeamFilterHandle(FAxGameplayTargetDataTeamFilter Filter, AActor* FilterActor);

	UFUNCTION(BlueprintPure, Category = "Filter")
	static FGameplayTargetDataFilterHandle MakeAxInterfaceFilterHandle(FAxGameplayTargetDataInterfaceFilter Filter, AActor* FilterActor);

	UFUNCTION(BlueprintCallable, Category="Rendering|Debug", meta=(WorldContext="WorldContextObject", DevelopmentOnly))
	static void DrawDebugHitResult(const UObject* WorldContextObject, const FHitResult& HitResult);

};
