#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PostInitializeAttributesInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPostInitializeAttributesInterface : public UInterface
{
	GENERATED_BODY()
};

class AXCOMMON_API IPostInitializeAttributesInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = Gameplay)
	virtual void PostInitialize() {};
};
