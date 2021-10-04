#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AxInteractionTargetActor.generated.h"

class APlayerController;
class UGameplayAbility;

UCLASS(Blueprintable, abstract, notplaceable)
class AXINTERACTIONS_API AAxInteractionTargetActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true), Replicated, Category = Targeting)
	FGameplayAbilityTargetingLocationInfo StartLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	TObjectPtr<APlayerController> MasterPC;

	UPROPERTY()
	TObjectPtr<UGameplayAbility> OwningAbility;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = Targeting)
	TObjectPtr<AActor> SourceActor;

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn = true), Category = Targeting)
	FGameplayTargetDataFilterHandle Filter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, meta = (ExposeOnSpawn = true), Category = Targeting)
	bool bDebug;

public:
	AAxInteractionTargetActor();
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	virtual void StartTargeting(UGameplayAbility* Ability);

	virtual void CheckForInteractionTarget(FHitResult& OutHitResult);

	FGameplayAbilityTargetDataHandle MakeTargetData(const FHitResult& HitResult) const;

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) PURE_VIRTUAL(AAxTargetActor, return FHitResult(););
};
