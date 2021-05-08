// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Engine/EngineTypes.h"
#include "AxWeaponBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AXWEAPONS_API UAxWeaponBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Targeting, Meta=(ExpandBoolAsExecs="ReturnValue"))
	static bool FindBlockingHitResult(FGameplayAbilityTargetDataHandle TargetData, FHitResult& OutBlockingHitResult);

	UFUNCTION(BlueprintPure)
	static float GetMeleeHitDirection(AActor* Target, AActor* Source);

	UFUNCTION(BlueprintPure)
	static FVector GetMeleeHitVector(AActor* Target, AActor* Source);
};
