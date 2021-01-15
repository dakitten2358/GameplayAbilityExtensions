// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

#if ENABLE_DRAW_DEBUG
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"

namespace AxDebug
{
	void AXCOMMON_API DrawLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = -1.0f);
	void AXCOMMON_API DrawLineTraceMulti(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = -1.0f);
	void AXCOMMON_API DrawDirection(const UWorld* World, const FVector& Start, const FVector& Direction, EDrawDebugTrace::Type DrawDebugType, float Distance = 2000.f, FLinearColor DirectionColor = FLinearColor::Yellow, float DrawTime = -1.0f);
};
#endif // ENABLE_DRAW_DEBUG