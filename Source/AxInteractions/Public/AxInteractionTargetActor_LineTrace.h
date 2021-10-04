#pragma once

#include "CoreMinimal.h"
#include "AxInteractionTargetActor_Trace.h"
#include "Engine/CollisionProfile.h"
#include "AxInteractionTargetActor_LineTrace.generated.h"

UCLASS(Blueprintable, notplaceable, config = Game)
class AXINTERACTIONS_API AAxInteractionTargetActor_LineTrace : public AAxInteractionTargetActor_Trace
{
	GENERATED_BODY()

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
