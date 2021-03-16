// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AxRepeatableTriggerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAxRepeatableTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AXWEAPONS_API IAxRepeatableTriggerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Weapon)
	void Trigger();
};
