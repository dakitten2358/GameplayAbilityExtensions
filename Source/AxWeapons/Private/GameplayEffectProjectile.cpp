// (C) 2021 Jay Murray


#include "GameplayEffectProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"

FName AGameplayEffectProjectile::ProjectileMovementComponentName(TEXT("ProjectileMoveComp"));
FName AGameplayEffectProjectile::CapsuleComponentName(TEXT("CollisionCylinder"));

AGameplayEffectProjectile::AGameplayEffectProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(AGameplayEffectProjectile::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(22.0f, 44.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::DefaultProjectile_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(AGameplayEffectProjectile::ProjectileMovementComponentName);
	ProjectileMovement->InitialSpeed = 10000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AGameplayEffectProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor == GetInstigator())
		return;

	Super::NotifyActorBeginOverlap(OtherActor);
}

void AGameplayEffectProjectile::AddTarget(AActor* TargetActor)
{
	ApplyEffects.AddTargetActor(TargetActor);
}

void AGameplayEffectProjectile::AddHitResult(const FHitResult& HitResult)
{
	ApplyEffects.AddTargetHitResult(HitResult);
}

void AGameplayEffectProjectile::K2_ExecuteGameplayCueWithParams(FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	if (OwningAbilitySystemComponent)
	{
		OwningAbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);	
	}
	else
	{
		UE_LOG(LogAxWeapons, Error, TEXT("%s tried to ExecuteGameplayCueWithParams but didn't have an OwningAbilitySystemComponent"), *AActor::GetDebugName(this));
	}
}
