#include "ModularCharacter.h"
#include "AxAbilitySystemComponent.h"
#include "GameFramework/Controller.h"
#include "ModularPlayerState.h"
#include "ModularAIController.h"
#include "Components/GameFrameworkComponentManager.h"

AModularCharacter::AModularCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, AbilitySystemComponent(nullptr)
	, bIsAbilitySystemBoundToInput(false)
{}

void AModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularCharacter::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void AModularCharacter::PossessedBy(AController* NewController)
{
	// if we're possessed by a player, we should get the ability system from the player state
	AModularPlayerState* ModularPlayerState = NewController->GetPlayerState<AModularPlayerState>();
	if (ModularPlayerState)
		ReplaceAbilitySystemComponent(ModularPlayerState, Cast<UAxAbilitySystemComponent>(ModularPlayerState->GetAbilitySystemComponent()));
	// if we're possessed by an ai, we can ask the ai for the ability
	else if (AModularAIController* ModularAIController = Cast<AModularAIController>(NewController))
		ReplaceAbilitySystemComponent(ModularAIController, Cast<UAxAbilitySystemComponent>(ModularAIController->GetAbilitySystemComponent()));

	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		BindAbilitySystemToInput();
		OnAbilitySystemAvailable();
	}
}

void AModularCharacter::UnPossessed()
{
	UnbindAbilitySystemFromInput();
	OnAbilitySystemUnavailable();
}

void AModularCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AModularPlayerState* ModularPlayerState = GetPlayerState<AModularPlayerState>();
	if (ModularPlayerState)
	{
		ReplaceAbilitySystemComponent(ModularPlayerState, Cast<UAxAbilitySystemComponent>(ModularPlayerState->GetAbilitySystemComponent()));
		if (AbilitySystemComponent)
		{
			BindAbilitySystemToInput();
			OnAbilitySystemAvailable();
		}
	}
}

void AModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindAbilitySystemToInput();
}

void AModularCharacter::ReplaceAbilitySystemComponent(AActor* NewOwner, UAxAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent && (NewAbilitySystemComponent != AbilitySystemComponent))
	{
		UnbindAbilitySystemFromInput();
		OnAbilitySystemUnavailable();
	}

	AbilitySystemComponent = NewAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(NewOwner, this);
}

void AModularCharacter::BindAbilitySystemToInput()
{
	if (!bIsAbilitySystemBoundToInput && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UAxAbilitySystemComponent* AxAbilitySystem = Cast< UAxAbilitySystemComponent>(AbilitySystemComponent);
		AxAbilitySystem->BindAbilityActivationToEnhancedInputComponent(InputComponent);
		bIsAbilitySystemBoundToInput = true;
	}
}

void AModularCharacter::UnbindAbilitySystemFromInput()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
		AbilitySystemComponent->UnbindAbilityActivateFromEnhancedInputComponent(InputComponent);
	bIsAbilitySystemBoundToInput = false;
}

void AModularCharacter::OnAbilitySystemAvailable()
{
	check(AbilitySystemComponent);
	K2_OnAbilitySystemAvailable();
}

void AModularCharacter::OnAbilitySystemUnavailable()
{
	K2_OnAbilitySystemUnavailable();
}

UAbilitySystemComponent* AModularCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AModularCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AModularCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
	return false;
}

bool AModularCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
	return false;
}

bool AModularCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
	return false;
}
