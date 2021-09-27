#pragma once

#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "ModularPlayerState.generated.h"

/** Minimal class that supports extension by game feature plugins */
UCLASS(Blueprintable)
class AXMODULARGAMEPLAY_API AModularPlayerState : public APlayerState, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	static const FName AbilitySystemComponentName;

private:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

public:
	AModularPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer());

	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	//~ End AActor interface

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
	//~ Begin APlayerState interface
	virtual void CopyProperties(APlayerState* PlayerState);
	//~ End APlayerState interface
};
