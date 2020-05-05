#include "Abilities/GameplayAbilityTargetActor_LineTrace.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "AxCommonDebug.h"

AGameplayAbilityTargetActor_LineTrace::AGameplayAbilityTargetActor_LineTrace()
	: AimDirection(EAimDirection::PlayerController)
	, MaxRange(999999.0f)
	, bTraceAffectsAimPitch(true)
	, MaxHitResultsPerTrace(1)
	, NumberOfTraces(1)
	, bIgnoreBlockingHits(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

// -- AGameplayAbilityTargetActor
void AGameplayAbilityTargetActor_LineTrace::StartTargeting(UGameplayAbility* Ability)
{
	SetActorTickEnabled(true);

	OwningAbility = Ability;
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

	// This is a lazy way of emptying and repopulating the ReticleActors.
	// We could come up with a solution that reuses them.
	DestroyReticleActors();

	if (ReticleClass)
	{
		for (int32 i = 0; i < MaxHitResultsPerTrace * NumberOfTraces; i++)
		{
			SpawnReticleActor(GetActorLocation(), GetActorRotation());
		}
	}
}

void AGameplayAbilityTargetActor_LineTrace::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (SourceActor)
	{
		TArray<FHitResult> HitResults = PerformTrace(SourceActor);
		FGameplayAbilityTargetDataHandle Handle = MakeTargetData(HitResults);
		TargetDataReadyDelegate.Broadcast(Handle);

#if ENABLE_DRAW_DEBUG
		if (bDebug)
		{
			ShowDebugTrace(SourceActor, HitResults, EDrawDebugTrace::ForDuration, 2.0f);
		}
#endif
	}
}

void AGameplayAbilityTargetActor_LineTrace::CancelTargeting()
{
	const FGameplayAbilityActorInfo* ActorInfo = (OwningAbility ? OwningAbility->GetCurrentActorInfo() : nullptr);
	UAbilitySystemComponent* AbilitySystemComponent = (ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityReplicatedEventDelegate(
			EAbilityGenericReplicatedEvent::GenericCancel, 
			OwningAbility->GetCurrentAbilitySpecHandle(), 
			OwningAbility->GetCurrentActivationInfo().GetActivationPredictionKey())
			.Remove(GenericCancelHandle);
	}
	else
	{
		UE_LOG(LogAxCommon, Error, TEXT("AGameplayAbilityTargetActor_LineTrace called with a null AbilitySystemComponent for Actor %s"), *GetName());
	}

	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());

	SetActorTickEnabled(false);
}

// -- AActor
void AGameplayAbilityTargetActor_LineTrace::BeginPlay()
{
	Super::BeginPlay();

	// Start with Tick disabled. We'll enable it in StartTargeting() and disable it again in StopTargeting().
	// For instant confirmations, tick will never happen because we StartTargeting(), ConfirmTargeting(), and immediately StopTargeting().
	SetActorTickEnabled(false);
}

void AGameplayAbilityTargetActor_LineTrace::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyReticleActors();
	Super::EndPlay(EndPlayReason);
}

void AGameplayAbilityTargetActor_LineTrace::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if ENABLE_DRAW_DEBUG
	if (SourceActor && bDebug)
	{
		TArray<FHitResult> HitResults;
		HitResults = PerformTrace(SourceActor);
		ShowDebugTrace(SourceActor, HitResults, EDrawDebugTrace::ForOneFrame);
	}
#endif
}

TArray<FHitResult> AGameplayAbilityTargetActor_LineTrace::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bIgnoreBlocks = bIgnoreBlockingHits;

	const FTwoVectors& TraceLine = GetTraceLine(InSourceActor, Params);
	TArray<FHitResult> ReturnHitResults;

	for (int32 TraceIndex = 0; TraceIndex < NumberOfTraces; TraceIndex++)
	{
		FVector TraceStart = TraceLine.v1;
		FVector TraceEnd = TraceLine.v2;
		
		// Move _this_ actor to the end location (why?)
		SetActorLocationAndRotation(TraceEnd, SourceActor->GetActorRotation());
		
		TArray<FHitResult> TraceHitResults;
		DoTrace(TraceHitResults, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);

		// filter out any excess hit results
		TraceHitResults.SetNum(FMath::Min(TraceHitResults.Num(), MaxHitResultsPerTrace));

		// update reticles
		UpdateReticleActorsFromHitResults(TraceIndex, TraceHitResults);

		// If there were no hits, add a default HitResult at the end of the trace
		if (TraceHitResults.Num() < 1)
		{
			FHitResult HitResult;
			// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
			HitResult.TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			HitResult.TraceEnd = TraceEnd;
			HitResult.Location = TraceEnd;
			HitResult.ImpactPoint = TraceEnd;
			TraceHitResults.Add(HitResult);
		}

		ReturnHitResults.Append(TraceHitResults);
	}

	return ReturnHitResults;
}

FGameplayAbilityTargetDataHandle AGameplayAbilityTargetActor_LineTrace::MakeTargetData(const TArray<FHitResult>& HitResults) const
{
	FGameplayAbilityTargetDataHandle ReturnDataHandle;

	for (int32 i = 0; i < HitResults.Num(); i++)
	{
		/** Note: These are cleaned up by the FGameplayAbilityTargetDataHandle (via an internal TSharedPtr) */
		FGameplayAbilityTargetData_SingleTargetHit* ReturnData = new FGameplayAbilityTargetData_SingleTargetHit();
		ReturnData->HitResult = HitResults[i];
		ReturnDataHandle.Add(ReturnData);
	}

	return ReturnDataHandle;
}

void AGameplayAbilityTargetActor_LineTrace::DoTrace(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle& FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams& Params) const
{
	// if you need to adjust the trace start/end, this is the place to do it in a drevied class
	LineTraceWithFilter(OutHitResults, World, Filter, Start, End, TraceProfile.Name, Params);
}

bool AGameplayAbilityTargetActor_LineTrace::LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const UWorld* World, const FGameplayTargetDataFilterHandle& FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams& Params) const
{
	check(World);

	TArray<FHitResult> HitResults;
	bool bAnyHits = World->LineTraceMultiByProfile(HitResults, Start, End, ProfileName, Params);

	TArray<FHitResult> FilteredHitResults;
	if (bAnyHits)
	{
		// Start param could be player ViewPoint. We want HitResult to always display the StartLocation.
		FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
		for (int32 HitIdx = 0; HitIdx < HitResults.Num(); ++HitIdx)
		{
			FHitResult& Hit = HitResults[HitIdx];
			if (!Hit.Actor.IsValid() || FilterHandle.FilterPassesForActor(Hit.Actor))
			{
				Hit.TraceStart = TraceStart;
				Hit.TraceEnd = End;

				FilteredHitResults.Add(Hit);
			}
		}
	}

	OutHitResults = FilteredHitResults;
	return bAnyHits;
}

FTwoVectors AGameplayAbilityTargetActor_LineTrace::GetTraceLine(AActor* InSourceActor, const FCollisionQueryParams& Params) const
{
	switch (AimDirection)
	{
	case EAimDirection::AvatarActor:
		return GetTraceLine_AvatarActor();
	case EAimDirection::PlayerController:
	default:
		return GetTraceLine_PlayerController(InSourceActor, Params);
	}	
}

FTwoVectors AGameplayAbilityTargetActor_LineTrace::GetTraceLine_AvatarActor() const
{
	FRotator ViewRot = OwningAbility->GetCurrentActorInfo()->AvatarActor->GetActorRotation();
	const FVector ViewDir = ViewRot.Vector();

	FVector Start = StartLocation.GetTargetingTransform().GetLocation();
	FVector End = Start + (ViewDir * MaxRange);
	
	return FTwoVectors(Start, End);
}

FTwoVectors AGameplayAbilityTargetActor_LineTrace::GetTraceLine_PlayerController(AActor* InSourceActor, const FCollisionQueryParams& Params) const
{
	const FVector Start = StartLocation.GetTargetingTransform().GetLocation();

	// Default values in case of AI Controller	
	FVector ViewStart = Start;
	FRotator ViewRot = StartLocation.GetTargetingTransform().GetRotation().Rotator();

	// Get the viewpoint from the PC
	APlayerController* PC = OwningAbility->GetCurrentActorInfo()->PlayerController.Get();
	if (PC)
	{
		PC->GetPlayerViewPoint(ViewStart, ViewRot);
	}

	const FVector ViewDir = ViewRot.Vector();
	FVector ViewEnd = ViewStart + (ViewDir * MaxRange);

	TArray<FHitResult> HitResults;
	bool bAnyHits = LineTraceWithFilter(HitResults, InSourceActor->GetWorld(), Filter, ViewStart, ViewEnd, TraceProfile.Name, Params);
	
	// figure out the direction from the start, not necessarily the view start
	const FVector AdjustedEnd = (bAnyHits) ? HitResults[0].Location : ViewEnd;
	FVector AdjustedAimDir = (AdjustedEnd - Start).GetSafeNormal();
	if (AdjustedAimDir.IsZero())
	{
		AdjustedAimDir = ViewDir;
	}

	// If we don't want to adjust the aim pitch, we need to fix it here
	if (!bTraceAffectsAimPitch && bAnyHits)
	{
		FVector OriginalAimDir = (ViewEnd - Start).GetSafeNormal();
		if (!OriginalAimDir.IsZero())
		{
			// Convert to angles and use original pitch
			const FRotator OriginalAimRot = OriginalAimDir.Rotation();

			FRotator AdjustedAimRot = AdjustedAimDir.Rotation();
			AdjustedAimRot.Pitch = OriginalAimRot.Pitch;

			AdjustedAimDir = AdjustedAimRot.Vector();
		}
	}

	const FVector End = Start + (AdjustedAimDir * MaxRange);
	return FTwoVectors(Start, End);
}

AGameplayAbilityWorldReticle* AGameplayAbilityTargetActor_LineTrace::SpawnReticleActor(FVector Location, FRotator Rotation)
{
	if (ReticleClass)
	{
		AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, Location, Rotation);
		if (SpawnedReticleActor)
		{
			SpawnedReticleActor->InitializeReticle(this, MasterPC, ReticleParams);
			SpawnedReticleActor->SetActorHiddenInGame(true);
			ReticleActors.Add(SpawnedReticleActor);

			// This is to catch cases of playing on a listen server where we are using a replicated reticle actor.
			// (In a client controlled player, this would only run on the client and therefor never replicate. If it runs
			// on a listen server, the reticle actor may replicate. We want consistancy between client/listen server players.
			// Just saying 'make the reticle actor non replicated' isnt a good answer, since we want to mix and match reticle
			// actors and there may be other targeting types that want to replicate the same reticle actor class).
			if (!ShouldProduceTargetDataOnServer)
			{
				SpawnedReticleActor->SetReplicates(false);
			}

			return SpawnedReticleActor;
		}
	}

	return nullptr;
}

void AGameplayAbilityTargetActor_LineTrace::DestroyReticleActors()
{
	for (int32 i = ReticleActors.Num() - 1; i >= 0; i--)
	{
		if (ReticleActors[i].IsValid())
		{
			ReticleActors[i].Get()->Destroy();
		}
	}

	ReticleActors.Empty();
}

void AGameplayAbilityTargetActor_LineTrace::UpdateReticleActorsFromHitResults(int32 TraceIndex, const TArray<FHitResult>& TraceHitResults)
{
	for (int32 j = TraceHitResults.Num() - 1; j >= 0; j--)
	{
		const FHitResult& HitResult = TraceHitResults[j];

		// update reticle actor
		int32 ReticleIndex = TraceIndex * MaxHitResultsPerTrace + j;
		UpdateReticleActorFromHitResult(ReticleIndex, HitResult);
	}

	if (TraceHitResults.Num() < ReticleActors.Num())
	{
		// We have less hit results than ReticleActors, hide the extra ones
		for (int32 j = TraceHitResults.Num(); j < ReticleActors.Num(); j++)
		{
			if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[j].Get())
			{
				LocalReticleActor->SetIsTargetAnActor(false);
				LocalReticleActor->SetActorHiddenInGame(true);
			}
		}
	}
}

void AGameplayAbilityTargetActor_LineTrace::UpdateReticleActorFromHitResult(int32 ReticleIndex, const FHitResult& HitResult)
{
	if (ReticleIndex < ReticleActors.Num())
	{
		if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActors[ReticleIndex].Get())
		{
			const bool bHitActor = HitResult.Actor != nullptr;

			if (bHitActor && !HitResult.bBlockingHit)
			{
				LocalReticleActor->SetActorHiddenInGame(false);

				const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? HitResult.Actor->GetActorLocation() : HitResult.Location;

				LocalReticleActor->SetActorLocation(ReticleLocation);
				LocalReticleActor->SetIsTargetAnActor(bHitActor);
			}
			else
			{
				LocalReticleActor->SetActorHiddenInGame(true);
			}
		}
	}
}

FCollisionQueryParams AGameplayAbilityTargetActor_LineTrace::CreateCollisionQueryParams(AActor* InSourceActor) const
{
	bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(InSourceActor);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_LineTrace), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActors(ActorsToIgnore);
	Params.bIgnoreBlocks = bIgnoreBlockingHits;

	return Params;
}

#if ENABLE_DRAW_DEBUG
void AGameplayAbilityTargetActor_LineTrace::ShowDebugTrace(AActor* InSourceActor, const TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration) const
{
	const FCollisionQueryParams Params = CreateCollisionQueryParams(InSourceActor);
	const FTwoVectors& TraceLine = GetTraceLine(InSourceActor, Params);

	bool bHit = HitResults.Num() > 0;

	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;

	AxDebug::DrawLineTraceMulti(GetWorld(), TraceLine.v1, TraceLine.v2, DrawDebugType, bHit, HitResults, TraceColor, TraceHitColor, Duration);
}
#endif