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
