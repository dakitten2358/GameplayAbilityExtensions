// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AxGameplayEffectContainer.h"
#include "GameplayEffectProjectile.generated.h"

UCLASS()
class AXWEAPONS_API AGameplayEffectProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadWrite, Category = "Effects", meta=(ExposeOnSpawn=true))
	FAxGameplayEffectContainerSpec ApplyEffects;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Gameplay", meta=(ExposeOnSpawn=true))
	TObjectPtr<class UAbilitySystemComponent> OwningAbilitySystemComponent;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Gameplay", meta=(ExposeOnSpawn=true))
	TObjectPtr<class AActor> EffectCauser;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

private:
	static FName CapsuleComponentName;
	static FName ProjectileMovementComponentName;

public:
	AGameplayEffectProjectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer());

	//~Begin AActor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	//~End AActor

protected:
	UFUNCTION(BlueprintCallable)
	void AddTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void AddHitResult(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category = Ability, meta = (GameplayTagFilter = "GameplayCue"), DisplayName = "Execute GameplayCueWithParams On Owner", meta=(ScriptName = "ExecuteGameplayCueWithParams"))
	void K2_ExecuteGameplayCueWithParams(FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
};
