#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AxGameplayAbilitySet.h"
#include "AxGrantAbilitiesComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AXCOMMON_API UAxGrantAbilitiesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TObjectPtr<UAxGameplayAbilitySet> AbilitySet;

private:
	UPROPERTY()
	TMap<TWeakObjectPtr<AActor>, FAxGameplayAbilitySetHandles> GrantedAbilitySetHandles;

public:	
	UAxGrantAbilitiesComponent();

	UFUNCTION(BlueprintCallable)
	void GrantAbilities(AActor* ToActor, int Level = 1);

	UFUNCTION(BlueprintCallable)
	void RemoveAbilities(AActor* FromActor);
};
