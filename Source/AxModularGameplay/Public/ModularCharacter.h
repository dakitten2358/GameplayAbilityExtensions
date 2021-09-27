#pragma once

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "ModularCharacter.generated.h"

/** Minimal class that supports extension by game feature plugins */
UCLASS(Blueprintable)
class AXMODULARGAMEPLAY_API AModularCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<class UAxAbilitySystemComponent> AbilitySystemComponent;

	bool bIsAbilitySystemBoundToInput;

public:
	AModularCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer());

	//~ Begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface

	//~Begin APawn
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//~End APawn

	//~ Begin IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	//~ Begin IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	//~ End IGameplayTagAssetInterface

protected:
	virtual void OnAbilitySystemAvailable();
	virtual void OnAbilitySystemUnavailable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnAbilitySystemAvailable", ScriptName="OnAbilitySystemAvailable"))
	void K2_OnAbilitySystemAvailable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="OnAbilitySystemUnavailable", ScriptName="OnAbilitySystemUnavailable"))
	void K2_OnAbilitySystemUnavailable();

private:
	void ReplaceAbilitySystemComponent(AActor* NewOwner, class UAxAbilitySystemComponent* NewAbilitySystemComponent);
	void BindAbilitySystemToInput();
	void UnbindAbilitySystemFromInput();
};
