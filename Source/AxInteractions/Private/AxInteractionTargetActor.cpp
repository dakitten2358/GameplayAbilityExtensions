#include "AxInteractionTargetActor.h"
#include "Abilities/GameplayAbility.h"
#include "Net/UnrealNetwork.h"

AAxInteractionTargetActor::AAxInteractionTargetActor()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}

void AAxInteractionTargetActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAxInteractionTargetActor, StartLocation);
	DOREPLIFETIME(AAxInteractionTargetActor, SourceActor);
	DOREPLIFETIME(AAxInteractionTargetActor, bDebug);

	DISABLE_REPLICATED_PROPERTY(AAxInteractionTargetActor, Filter);
}

void AAxInteractionTargetActor::StartTargeting(class UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AAxInteractionTargetActor::CheckForInteractionTarget(FHitResult& OutHitResult)
{
	OutHitResult = FHitResult();

	if (SourceActor)
	{
		FHitResult HitResult = PerformTrace(SourceActor);
		FVector EndPoint = HitResult.Component.IsValid() ? HitResult.ImpactPoint : HitResult.TraceEnd;
		SetActorLocationAndRotation(EndPoint, SourceActor->GetActorRotation());
		OutHitResult = HitResult;
	}
}

FGameplayAbilityTargetDataHandle AAxInteractionTargetActor::MakeTargetData(const FHitResult& HitResult) const
{
	if (HitResult.IsValidBlockingHit())
	{
		/** Note: This will be cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		return StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
	}

	TArray<FHitResult> EmptyHitResults;
	return StartLocation.MakeTargetDataHandleFromHitResults(OwningAbility, EmptyHitResults);
}
