#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Engine/EngineTypes.h"
#include "GameplayEffectTypes.h"
#include "AxGameplayEffectContainer.generated.h"

USTRUCT(BlueprintType)
struct AXCOMMON_API FAxGameplayEffectContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TSubclassOf<class UAxGameplayTargetProvider> TargetProvider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<TSubclassOf<class UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct AXCOMMON_API FAxGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	TMap<FGameplayTag, float> SetByCallerTagMagnitudes;

	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	/** Add a single FGameplayAbilityTargetDataHandle to the target data */
	void AddTargetData(const FGameplayAbilityTargetDataHandle& InTargetData);

	void AddTargetActor(const TWeakObjectPtr<AActor>& TargetActor);
	void AddTargetHitResult(const FHitResult& HitResult);

	/** Clears target data */
	void ClearTargets();

	void SetSetByCallerMagnitude(FGameplayTag DataTag, float Magnitude);
};
