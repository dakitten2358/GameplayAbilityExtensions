#include "ModularPlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/ControllerComponent.h"
#include "AbilitySystemInterface.h"

void AModularPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void AModularPlayerController::ReceivedPlayer()
{
	// Player controllers always get assigned a player and can't do much until then
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::ReceivedPlayer();

	TArray<UControllerComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UControllerComponent* Component : ModularComponents)
	{
		Component->ReceivedPlayer();
	}
}

void AModularPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	TArray<UControllerComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UControllerComponent* Component : ModularComponents)
	{
		Component->PlayerTick(DeltaTime);
	}
}

bool AModularPlayerController::IsLookInputIgnored() const
{
	bool isLookInputIgnored = Super::IsLookInputIgnored();
	if (IgnoreLookInputTag.IsValid())
	{
		if (IAbilitySystemInterface* AbilitySystemInterface = GetPlayerState<IAbilitySystemInterface>())
		{
			if (AbilitySystemInterface->GetAbilitySystemComponent()->HasMatchingGameplayTag(IgnoreLookInputTag))
				isLookInputIgnored = true;
		}
	}
	return isLookInputIgnored;
}

bool AModularPlayerController::IsMoveInputIgnored() const
{
	bool isMoveInputIgnored = Super::IsMoveInputIgnored();
	if (IgnoreMoveInputTag.IsValid())
	{
		if (IAbilitySystemInterface* AbilitySystemInterface = GetPlayerState<IAbilitySystemInterface>())
		{
			if (AbilitySystemInterface->GetAbilitySystemComponent()->HasMatchingGameplayTag(IgnoreMoveInputTag))
				isMoveInputIgnored = true;
		}
	}
	return isMoveInputIgnored;
}
