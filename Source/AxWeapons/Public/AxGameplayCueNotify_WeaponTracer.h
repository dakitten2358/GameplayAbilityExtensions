// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GameplayEffectTypes.h"
#include "AxGameplayCueNotify_WeaponTracer.generated.h"

/**
 * 
 */
UCLASS()
class AXWEAPONS_API UAxGameplayCueNotify_WeaponTracer : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "GameplayCueNotify", meta=(DisplayName="OnExecute_Hit", ScriptName="OnExecute_Hit"))
	void K2_OnExecute_Hit(AActor* MyTarget, const FGameplayCueParameters& Parameters, const FHitResult& HitResult) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "GameplayCueNotify", meta=(DisplayName="OnExecute_Trace", ScriptName="OnExecute_Trace"))
	void K2_OnExecute_Trace(AActor* MyTarget, const FGameplayCueParameters& Parameters, const FHitResult& HitResult) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="GameplayCueNotify", meta=(Keywords="niagara System", WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	class UNiagaraComponent* SpawnWeaponTracer(class UNiagaraSystem* SystemTemplate, const FHitResult& HitResult, FVector SpawnOffset, float& OutDistance, FVector& OutStart) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="GameplayCueNotify", meta=(Keywords="niagara System", WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	class UNiagaraComponent* SpawnWeaponHit(class UNiagaraSystem* SystemTemplate, const FHitResult& HitResult) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category="GameplayCueNotify", meta=(Keywords="niagara System", WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	class UNiagaraComponent* SpawnWeaponMuzzleFlash(class UNiagaraSystem* SystemTemplate, class USceneComponent* AttachToComponent, FName Socket, const FHitResult& HitResult) const;
};
