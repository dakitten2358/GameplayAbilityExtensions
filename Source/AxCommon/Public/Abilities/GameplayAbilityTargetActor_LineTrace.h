// Heavily influenced/borrowed from GASShooter

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayAbilityTargetActor_LineTrace.generated.h"

class AGameplayAbilityWorldReticle;

UENUM(Meta=(ToolTip="What to use as the direction"))
enum class EAimDirection : uint8
{
	AvatarActor UMETA(DisplayName = "Avatar Direction", ToolTip="From start location in the direction the avatar is facing"),
	PlayerController UMETA(DisplayName = "Look Direction", ToolTip="From start location, in the direction the camera is facing, involves an additional trace"),
	StartLocation UMETA(DisplayName = "Start Location", ToolTip="Use the rotation from StartLocation as the direction of the trace"),
};

UCLASS()
class AXCOMMON_API AGameplayAbilityTargetActor_LineTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	EAimDirection AimDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	float MaxRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = Trace)
	FCollisionProfileName TraceProfile;

	// Does the trace affect the aiming pitch
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	bool bTraceAffectsAimPitch;

	// Maximum hit results to return per trace. 0 just returns the trace end point.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	int32 MaxHitResultsPerTrace;

	// Number of traces to perform at one time. Single bullet weapons like rilfes will only do one trace.
	// Multi-bullet weapons like shotguns can do multiple traces. Not intended to be used with PersistentHits.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	int32 NumberOfTraces;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true, ToolTip="Horizontal spread, in degrees"), Category = Trace)
	float Spread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true, ToolTip = "Vertical spread, in degrees"), Category = Trace)
	float VSpread;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	bool bIgnoreBlockingHits;

public:
	AGameplayAbilityTargetActor_LineTrace();

	// -- AGameplayAbilityTargetActor
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	// -- AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	TArray<FHitResult> PerformTrace(AActor* InSourceActor);
	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>& HitResults) const;

protected:
	virtual void DoTrace(TArray<FHitResult>& OutHitResults, const class UWorld* World, const FGameplayTargetDataFilterHandle& FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams& Params) const;

	bool LineTraceWithFilter(TArray<FHitResult>& OutHitResults, const class UWorld* World, const FGameplayTargetDataFilterHandle& FilterHandle, const FVector& Start, const FVector& End, FName ProfileName, const FCollisionQueryParams& Params) const;

	FTwoVectors GetTraceLine(AActor* InSourceActor, const FCollisionQueryParams& Params) const;
	FTwoVectors GetTraceLine_AvatarActor() const;
	FTwoVectors GetTraceLine_PlayerController(AActor* InSourceActor, const FCollisionQueryParams& Params) const;
	FTwoVectors GetTraceLine_StartLocation() const;

	FCollisionQueryParams CreateCollisionQueryParams(AActor* InSourceActor) const;

	virtual FTwoVectors AdjustTraceForSpread(int32 TraceIndex, const FTwoVectors& TraceLine);

private:
	TArray<TWeakObjectPtr<class AGameplayAbilityWorldReticle>> ReticleActors;
	class AGameplayAbilityWorldReticle* SpawnReticleActor(FVector Location, FRotator Rotation);
	void DestroyReticleActors();
	void UpdateReticleActorsFromHitResults(int32 TraceIndex, const TArray<FHitResult>& TraceHitResults);
	void UpdateReticleActorFromHitResult(int32 ReticleIndex, const FHitResult& HitResult);
	bool IsAlreadyInHitResults(const TArray<FHitResult>& HitResults, TWeakObjectPtr<AActor> ActorToCheckPtr) const;

#if ENABLE_DRAW_DEBUG
	void ShowDebugTrace(AActor* InSourceActor, const TArray<FHitResult>& HitResults, EDrawDebugTrace::Type DrawDebugType, float Duration = -1.0f) const;
#endif
};
