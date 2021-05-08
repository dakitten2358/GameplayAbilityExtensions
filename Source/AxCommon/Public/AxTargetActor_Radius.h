// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "Engine/CollisionProfile.h"
#include "AxTargetActor_Radius.generated.h"

UCLASS(Blueprintable, notplaceable)
class AXCOMMON_API AAxTargetActor_Radius : public AGameplayAbilityTargetActor
{
	GENERATED_UCLASS_BODY()

public:

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	
	virtual void ConfirmTargetingAndContinue() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, config, meta = (ExposeOnSpawn = true), Category = Trace)
	FCollisionProfileName TraceProfile;

	/** Radius of target acquisition around the ability's start location. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Radius)
	float Radius;

protected:

	TArray<TWeakObjectPtr<AActor> >	PerformOverlap(const FVector& Origin);

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;
};
