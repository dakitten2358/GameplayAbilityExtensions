#pragma once

#include "CoreMinimal.h"
#include "AxInteractionTargetActor.h"
#include "DrawDebugHelpers.h"
#if ENABLE_DRAW_DEBUG
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"
#endif
#include "AxInteractionTargetActor_Trace.generated.h"

UENUM(BlueprintType)
enum class ETargetTraceSource : uint8
{
	Controller,
	Avatar,
};

UCLASS(Abstract, Blueprintable, notplaceable, config = Game)
class AXINTERACTIONS_API AAxInteractionTargetActor_Trace : public AAxInteractionTargetActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	ETargetTraceSource TraceSource;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Trace)
	float MaxRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

protected:
	void LineTraceWithFilter(FHitResult& OutHitResult, const UWorld* World, const FGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End, ECollisionChannel Channel, const FCollisionQueryParams Params) const;

	void AimWithPlayerController(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	void AimWithAvatar(const AActor* InSourceActor, FCollisionQueryParams Params, const FVector& TraceStart, FVector& OutTraceEnd, bool bIgnorePitch = false) const;
	static bool ClipCameraRayToAbilityRange(FVector CameraLocation, FVector CameraDirection, FVector AbilityCenter, float AbilityRange, FVector& ClippedPosition);

#if ENABLE_DRAW_DEBUG
	void DrawLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = -1.0f);
#endif
};
