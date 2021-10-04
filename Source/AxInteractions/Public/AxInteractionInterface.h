#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AxInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAxInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class AXINTERACTIONS_API IAxInteractionInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	bool IsInteractionTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	FGameplayTag GetStartInteractionEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void StartInteractionHighlighting();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void StopInteractionHighlighting();
};
