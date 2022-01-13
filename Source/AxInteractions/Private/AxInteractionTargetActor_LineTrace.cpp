#include "AxInteractionTargetActor_LineTrace.h"

FHitResult AAxInteractionTargetActor_LineTrace::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_SingleLineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();// InSourceActor->GetActorLocation();
	FVector TraceEnd;

	if (TraceSource == ETargetTraceSource::Controller)
		AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd);		//Effective on server and launching client only
	else
		AimWithAvatar(InSourceActor, Params, TraceStart, TraceEnd);

	// ------------------------------------------------------

	FHitResult ReturnHitResult;
	LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceChannel, Params);
	//Default to end of trace line if we don't hit anything.
	if (!ReturnHitResult.bBlockingHit)
	{
		ReturnHitResult.Location = TraceEnd;
	}

#if ENABLE_DRAW_DEBUG
	if (bDebug && false)
	{
		DrawLineTraceSingle(GetWorld(), TraceStart, TraceEnd, EDrawDebugTrace::Type::ForDuration, ReturnHitResult.bBlockingHit, ReturnHitResult, FLinearColor::Red, FLinearColor::Green, 0.1f);
	}
#endif // ENABLE_DRAW_DEBUG
	return ReturnHitResult;
}


