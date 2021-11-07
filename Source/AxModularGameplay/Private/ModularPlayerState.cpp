#include "ModularPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"
#include "AxAbilitySystemComponent.h"

const FName AModularPlayerState::AbilitySystemComponentName(TEXT("AbilitySystemComponent"));

AModularPlayerState::AModularPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UAxAbilitySystemComponent>(AbilitySystemComponentName);
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// By default PlayerState has an incredibly low update frequency (1), which causes very
	// noticeable delays in replication of gameplay tags, etc.
	//
	// For reference, the default is frequency of 100, and a priority of 1.  Character has a priority of 3,
	// and AInfo has a frequency of 10.
	NetUpdateFrequency = 20;
	NetPriority = 2;	
}

void AModularPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AModularPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void AModularPlayerState::Reset()
{
	Super::Reset();

	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UPlayerStateComponent* Component : ModularComponents)
	{
		Component->Reset();
	}
}

void AModularPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);

	TArray<UPlayerStateComponent*> OtherModularComponents;
	PlayerState->GetComponents(OtherModularComponents);

	for (UPlayerStateComponent* Component : ModularComponents)
	{
		for (UPlayerStateComponent* OtherComponent : OtherModularComponents)
		{
			Component->CopyProperties(OtherComponent);
		}
	}
}

UAbilitySystemComponent* AModularPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AModularPlayerState::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AModularPlayerState::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AModularPlayerState::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AModularPlayerState::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}
