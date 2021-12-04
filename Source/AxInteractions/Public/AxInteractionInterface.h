#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Abilities/GameplayAbilityTypes.h"
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
	bool IsInteractionTarget(const AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	FGameplayEventData GetStartInteractionEventData(const AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void OnInteractionCompleted();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void StartInteractionHighlighting(const AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
	void StopInteractionHighlighting(const AActor* Actor);
};
