// (C) 2021 Jay Murray


#include "AxTargetActor_Radius.h"
#include "GameFramework/Pawn.h"
#include "WorldCollision.h"
#include "Abilities/GameplayAbility.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	AAxTargetActor_Radius
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------

AAxTargetActor_Radius::AAxTargetActor_Radius(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	ShouldProduceTargetDataOnServer = true;
}

void AAxTargetActor_Radius::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);
	SourceActor = InAbility->GetCurrentActorInfo()->AvatarActor.Get();
}

void AAxTargetActor_Radius::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(PerformOverlap(Origin), Origin);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

FGameplayAbilityTargetDataHandle AAxTargetActor_Radius::MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
	if (OwningAbility)
	{
		/** Use the source location instead of the literal origin */
		return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
	}

	return FGameplayAbilityTargetDataHandle();
}

TArray<TWeakObjectPtr<AActor> >	AAxTargetActor_Radius::PerformOverlap(const FVector& Origin)
{
	bool bTraceComplex = false;
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RadiusTargetingOverlap), bTraceComplex);
	Params.bReturnPhysicalMaterial = false;

	TArray<FOverlapResult> Overlaps;

	SourceActor->GetWorld()->OverlapMultiByProfile(Overlaps, Origin, FQuat::Identity, TraceProfile.Name, FCollisionShape::MakeSphere(Radius), Params);

	TArray<TWeakObjectPtr<AActor>>	HitActors;

	for (int32 i = 0; i < Overlaps.Num(); ++i)
	{
		//Should this check to see if these pawns are in the AimTarget list?
		AActor* Actor = Overlaps[i].GetActor();
		if (Actor && !HitActors.Contains(Actor) && Filter.FilterPassesForActor(Actor))
		{
			HitActors.Add(Actor);
		}
	}

	return HitActors;
}
