// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAxCommon, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogAxAI, Log, All);

#if ENABLE_DRAW_DEBUG
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Color.h"

namespace AxDebug
{
	void DrawLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor = FLinearColor::Red, FLinearColor TraceHitColor = FLinearColor::Green, float DrawTime = -1.0f);
	void DrawDirection(const UWorld* World, const FVector& Start, const FVector& Direction, EDrawDebugTrace::Type DrawDebugType, float Distance = 2000.f, FLinearColor DirectionColor = FLinearColor::Yellow, float DrawTime = -1.0f);
};
#endif // ENABLE_DRAW_DEBUG