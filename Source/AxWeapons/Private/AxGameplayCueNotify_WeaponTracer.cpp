// (C) 2021 Jay Murray


#include "AxGameplayCueNotify_WeaponTracer.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AxBlueprintFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "AxCommonDrawDebug.h"

bool UAxGameplayCueNotify_WeaponTracer::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	 FGameplayAbilityTargetDataHandle TargetDataHandle = UAxBlueprintFunctionLibrary::EffectContextGetTargetData(Parameters.EffectContext);

	 const FHitResult* LastHitResult = nullptr;

	 // these are for the hit effects on each target
	 for(int TargetDataIndex = 0; TargetDataIndex < TargetDataHandle.Num(); ++TargetDataIndex)
	 {
	 	if (!TargetDataHandle.IsValid(TargetDataIndex))
	 		continue;

	 	FGameplayAbilityTargetData* TargetData = TargetDataHandle.Get(TargetDataIndex);
	 	if (!TargetData->HasHitResult())
	 		continue;

 		const FHitResult* HitResult = TargetData->GetHitResult();
 		K2_OnExecute_Hit(MyTarget, Parameters, *HitResult);
 		LastHitResult = HitResult;
	 }

	 // furthest trace is used to draw the actual trace
	 if (LastHitResult)
	 	K2_OnExecute_Trace(MyTarget, Parameters, *LastHitResult);

	 return true;
}

void UAxGameplayCueNotify_WeaponTracer::K2_OnExecute_Hit_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters, const FHitResult& HitResult) const {}
void UAxGameplayCueNotify_WeaponTracer::K2_OnExecute_Trace_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters, const FHitResult& HitResult) const {}

UNiagaraComponent* UAxGameplayCueNotify_WeaponTracer::SpawnWeaponTracer(class UNiagaraSystem* SystemTemplate, const FHitResult& HitResult, FVector SpawnOffset, float& OutDistance, FVector& OutStart) const
{
	FVector SpawnLocation = HitResult.TraceStart + SpawnOffset;
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, HitResult.Location);

	float Distance = (HitResult.Location - SpawnLocation).Size();

	FVector SpawnScale = FVector(1.f);
	bool bAutoDestroy = true;
	bool bAutoActivate = true;
	ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SystemTemplate, SpawnLocation, SpawnRotation, SpawnScale, bAutoDestroy, bAutoActivate, PoolingMethod);

	OutDistance = Distance;
	OutStart = SpawnLocation;
	return NiagaraComponent;
}

UNiagaraComponent* UAxGameplayCueNotify_WeaponTracer::SpawnWeaponHit(class UNiagaraSystem* SystemTemplate, const FHitResult& HitResult) const
{
	FVector SpawnLocation = HitResult.Location;

	FVector TraceDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
	FVector ReflectionDirection = FMath::GetReflectionVector(TraceDirection, HitResult.Normal);
	FRotator SpawnRotation = FRotationMatrix::MakeFromX(ReflectionDirection).Rotator();

	FVector SpawnScale = FVector(1.f);
	bool bAutoDestroy = true;
	bool bAutoActivate = true;
	ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SystemTemplate, SpawnLocation, SpawnRotation, SpawnScale, bAutoDestroy, bAutoActivate, PoolingMethod);
	return NiagaraComponent;
}

UNiagaraComponent* UAxGameplayCueNotify_WeaponTracer::SpawnWeaponMuzzleFlash(class UNiagaraSystem* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName, const FHitResult& HitResult) const
{
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	EAttachLocation::Type LocationType = EAttachLocation::SnapToTarget;
	bool bAutoDestroy = true;
	bool bAutoActivate = true;
	ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease;

	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplate, AttachToComponent, AttachPointName, Location, Rotation, LocationType, bAutoDestroy, bAutoActivate, PoolingMethod);
	return NiagaraComponent;
}