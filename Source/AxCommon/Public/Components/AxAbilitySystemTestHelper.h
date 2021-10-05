#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AxAbilitySystemTestHelper.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AXCOMMON_API UAxAbilitySystemTestHelper : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAxAbilitySystemTestHelper();

	UFUNCTION(BlueprintCallable)
	void InjectAbilityInputPress(const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	void InjectAbilityInputTrigger(const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable)
	void InjectAbilityInputRelease(const UInputAction* InputAction);
};
