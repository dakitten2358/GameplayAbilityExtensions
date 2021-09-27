#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "AxGameplayAbilitySet.generated.h"

// Similar to UGameplayAbilitySet, but removes the bindings,
// adds start up effects, and if it's a UAxGameplayAbility
// will use the binding from that.  Also provides a way to
// remove the abilities and effects (useful for weapons,
// equippables, etc)

USTRUCT()
struct FAxGameplayAbilitySetHandles
{
	GENERATED_BODY()

public:
	bool Any() const { return (AbilityHandles.Num() > 0) || (EffectHandles.Num() > 0); }

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	friend class UAxGameplayAbilitySet;
};

UCLASS()
class AXCOMMON_API UAxGameplayAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	void Give(class UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const;
	void Give(class AActor* Actor, UObject* SourceObject, int32 Level, FAxGameplayAbilitySetHandles& OutAbilitySetHandles) const;
	void Give(class UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject = nullptr, int32 Level = 1) const;
	void Give(class AActor* Actor, UObject* SourceObject = nullptr, int32 Level = 1) const;

	void Remove(class UAbilitySystemComponent* AbilitySystemComponent, FAxGameplayAbilitySetHandles& AbilitySetHandles) const;
	void Remove(class AActor* Actor, FAxGameplayAbilitySetHandles& AbilitySetHandles) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TArray<TSubclassOf<class UGameplayEffect>> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	bool bRemoveAbilitiesAfterActivation;
};
