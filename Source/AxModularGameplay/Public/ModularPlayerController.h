#pragma once

#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "ModularPlayerController.generated.h"

/** Minimal class that supports extension by game feature plugins */
UCLASS(Blueprintable)
class AXMODULARGAMEPLAY_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Input)
	FGameplayTag IgnoreLookInputTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Input)
	FGameplayTag IgnoreMoveInputTag;

public:
	//~ Begin AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor interface

	//~ Begin AController interface
	virtual bool IsLookInputIgnored() const;
	virtual bool IsMoveInputIgnored() const;
	//~ End AController interface

	//~ Begin APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	//~ End APlayerController interface
};
