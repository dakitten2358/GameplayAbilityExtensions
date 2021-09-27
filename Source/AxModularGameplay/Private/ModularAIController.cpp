#include "ModularAIController.h"
#include "AxAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"

AModularAIController::AModularAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAxAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AModularAIController::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularAIController::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void AModularAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

UAbilitySystemComponent* AModularAIController::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AModularAIController::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AModularAIController::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AModularAIController::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AModularAIController::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}
