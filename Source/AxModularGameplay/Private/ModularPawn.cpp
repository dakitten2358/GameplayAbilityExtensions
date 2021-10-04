#include "ModularPawn.h"
#include "AxAbilitySystemComponent.h"
#include "GameFramework/Controller.h"
#include "ModularPlayerState.h"
#include "ModularAIController.h"
#include "Components/GameFrameworkComponentManager.h"

AModularPawn::AModularPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, AbilitySystemComponent(nullptr)
	, bIsAbilitySystemBoundToInput(false)
{}

void AModularPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AModularPawn::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AModularPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void AModularPawn::PossessedBy(AController* NewController)
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

void AModularPawn::UnPossessed()
{
	UnbindAbilitySystemFromInput();
	OnAbilitySystemUnavailable();
}

void AModularPawn::OnRep_PlayerState()
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

void AModularPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindAbilitySystemToInput();
}

void AModularPawn::ReplaceAbilitySystemComponent(AActor* NewOwner, UAxAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent && (NewAbilitySystemComponent != AbilitySystemComponent))
	{
		UnbindAbilitySystemFromInput();
		OnAbilitySystemUnavailable();
	}

	AbilitySystemComponent = NewAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(NewOwner, this);
}

void AModularPawn::BindAbilitySystemToInput()
{
	if (!bIsAbilitySystemBoundToInput && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		UAxAbilitySystemComponent* AxAbilitySystem = Cast< UAxAbilitySystemComponent>(AbilitySystemComponent);
		AxAbilitySystem->BindAbilityActivationToEnhancedInputComponent(InputComponent);
		bIsAbilitySystemBoundToInput = true;
	}
}

void AModularPawn::UnbindAbilitySystemFromInput()
{
	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
		AbilitySystemComponent->UnbindAbilityActivateFromEnhancedInputComponent(InputComponent);
	bIsAbilitySystemBoundToInput = false;
}

void AModularPawn::OnAbilitySystemAvailable()
{
	check(AbilitySystemComponent);
	AddStartupGameplayAbilities();
	K2_OnAbilitySystemAvailable();
}

void AModularPawn::OnAbilitySystemUnavailable()
{
	RemoveStartupGameplayAbilities();
	K2_OnAbilitySystemUnavailable();
}

void AModularPawn::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if (GetLocalRole() == ROLE_Authority && StartupAbilitySet && !StartupAbilitySetHandles.Any())
	{
		StartupAbilitySet->Give(GetAbilitySystemComponent(), this, GetPawnLevel(), StartupAbilitySetHandles);
	}
}

void AModularPawn::RemoveStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if (GetLocalRole() == ROLE_Authority && StartupAbilitySet && StartupAbilitySetHandles.Any())
	{
		StartupAbilitySet->Remove(GetAbilitySystemComponent(), StartupAbilitySetHandles);
	}
}

int AModularPawn::GetPawnLevel() const
{
	return 1;
}

UAbilitySystemComponent* AModularPawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AModularPawn::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AModularPawn::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
	return false;
}

bool AModularPawn::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
	return false;
}

bool AModularPawn::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent)
		return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
	return false;
}
