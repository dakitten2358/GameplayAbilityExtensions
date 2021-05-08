// (C) 2021 Jay Murray


#include "AxWeaponBlueprintLibrary.h"

bool UAxWeaponBlueprintLibrary::FindBlockingHitResult(FGameplayAbilityTargetDataHandle TargetData, FHitResult& OutBlockingHitResult)
{
	OutBlockingHitResult = FHitResult();
	if (TargetData.Num() == 0)
		return false;

	for(int TargetDataIndex = 0; TargetDataIndex < TargetData.Num(); ++TargetDataIndex)
	{
		if (!TargetData.Data.IsValidIndex(TargetDataIndex))
			continue;

		FGameplayAbilityTargetData* Data = TargetData.Data[TargetDataIndex].Get();
		if (Data)
		{
			const FHitResult* HitResultPtr = Data->GetHitResult();
			if (HitResultPtr && HitResultPtr->bBlockingHit)
			{
				OutBlockingHitResult = *HitResultPtr;
				return true;
			}
		}
	}

	return false;
}

float UAxWeaponBlueprintLibrary::GetMeleeHitDirection(AActor* Target, AActor* Source)
{
	FVector Start = Source->GetActorLocation();
	FVector End = Target->GetActorLocation();
	FVector Direction = (End - Start);

	FRotator BaseRotation = Target->GetActorRotation();
	BaseRotation.Pitch = 0.f;
	BaseRotation.Roll = 0.f;

	if (!Direction.IsNearlyZero())
	{
		FMatrix RotMatrix = FRotationMatrix(BaseRotation);
		FVector ForwardVector = RotMatrix.GetScaledAxis(EAxis::X);
		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y);
		FVector NormalizedDirection = Direction.GetSafeNormal2D();

		// get a cos(alpha) of forward vector vs velocity
		float ForwardCosAngle = FVector::DotProduct(ForwardVector, NormalizedDirection);
		// now get the alpha and convert to degree
		float ForwardDeltaDegree = FMath::RadiansToDegrees(FMath::Acos(ForwardCosAngle));

		// depending on where right vector is, flip it
		float RightCosAngle = FVector::DotProduct(RightVector, NormalizedDirection);
		if (RightCosAngle < 0)
		{
			ForwardDeltaDegree *= -1;
		}

		return ForwardDeltaDegree;
	}

	return 0.f;
}

FVector UAxWeaponBlueprintLibrary::GetMeleeHitVector(AActor* Target, AActor* Source)
{
	FVector Start = Source->GetActorLocation();
	FVector End = Target->GetActorLocation();
	FVector Direction = (End - Start);
	return Direction.GetSafeNormal2D();
}