// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "AxAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AXCOMMON_API UAxAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void BindAbilityActivationToEnhancedInputComponent(UInputComponent* InputComponent);
	void UnbindAbilityActivateFromEnhancedInputComponent(UInputComponent* InputComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Abilities")
	FGameplayAbilitySpecHandle FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual bool TryActivateAbilityBatched(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately);

	// Exposes AddLooseGameplayTag to Blueprint. This tag is *not* replicated.
	UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "AddLooseGameplayTag"))
	void K2_AddLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count = 1);

	// Exposes AddLooseGameplayTags to Blueprint. These tags are *not* replicated.
	UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "AddLooseGameplayTags"))
	void K2_AddLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count = 1);

	// Exposes RemoveLooseGameplayTag to Blueprint. This tag is *not* replicated.
	UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "RemoveLooseGameplayTag"))
	void K2_RemoveLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count = 1);

	// Exposes RemoveLooseGameplayTags to Blueprint. These tags are *not* replicated.
	UFUNCTION(BlueprintCallable, Category = "Abilities", Meta = (DisplayName = "RemoveLooseGameplayTags"))
	void K2_RemoveLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count = 1);

private:
	void OnInputActionStarted(const UInputAction* InputAction);
	void OnInputActionTriggered(const UInputAction* InputAction);
	void OnInputActionEnded(const UInputAction* InputAction);

	void CacheInputActions();

	bool bHasCachedInputActions;

	UPROPERTY()
	TArray<const class UInputAction*> CachedInputActions;

	TMap<UInputComponent*, TArray<uint32>> BoundActionsByComponent;
};
