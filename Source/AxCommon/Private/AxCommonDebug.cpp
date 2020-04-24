// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AxCommonDebug.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogAxCommon);
DEFINE_LOG_CATEGORY(LogAxAI);

#if ENABLE_DRAW_DEBUG
namespace AxDebug
{
	static const float AX_TRACE_DEBUG_IMPACTPOINT_SIZE = 1.f;

	void DrawLineTraceSingle(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, bool bHit, const FHitResult& OutHit, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
	{
		if (DrawDebugType != EDrawDebugTrace::None)
		{
			bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
			float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

			// @fixme, draw line with thickness = 2.f?
			if (bHit && OutHit.bBlockingHit)
			{
				// Red up to the blocking hit, green thereafter
				::DrawDebugLine(World, Start, OutHit.ImpactPoint, TraceColor.ToFColor(true), bPersistent, LifeTime);
				::DrawDebugLine(World, OutHit.ImpactPoint, End, TraceHitColor.ToFColor(true), bPersistent, LifeTime);
				::DrawDebugPoint(World, OutHit.ImpactPoint, AX_TRACE_DEBUG_IMPACTPOINT_SIZE, TraceColor.ToFColor(true), bPersistent, LifeTime);
			}
			else
			{
				// no hit means all red
				::DrawDebugLine(World, Start, End, TraceColor.ToFColor(true), bPersistent, LifeTime);
			}
		}
	}

	void DrawDirection(const UWorld* World, const FVector& Start, const FVector& Direction, EDrawDebugTrace::Type DrawDebugType, float Distance, FLinearColor DirectionColor, float DrawTime)
	{
		if (DrawDebugType != EDrawDebugTrace::None)
		{
			bool bPersistent = DrawDebugType == EDrawDebugTrace::Persistent;
			float LifeTime = (DrawDebugType == EDrawDebugTrace::ForDuration) ? DrawTime : 0.f;

			FVector NormalizedDirection = Direction.GetSafeNormal();
			FVector End = Start + (NormalizedDirection * Distance);

			::DrawDebugLine(World, Start, End, DirectionColor.ToFColor(true), bPersistent, LifeTime);
		}
	}
}
#endif