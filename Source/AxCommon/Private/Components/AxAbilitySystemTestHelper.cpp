#include "Components/AxAbilitySystemTestHelper.h"
#include "AxAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

UAxAbilitySystemTestHelper::UAxAbilitySystemTestHelper()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAxAbilitySystemTestHelper::InjectAbilityInputPress(const UInputAction* InputAction)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner());
	UAxAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface ? Cast<UAxAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent()) : nullptr;
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnInputActionStarted(InputAction);
	}
}

void UAxAbilitySystemTestHelper::InjectAbilityInputTrigger(const UInputAction* InputAction)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner());
	UAxAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface ? Cast<UAxAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent()) : nullptr;
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnInputActionTriggered(InputAction);
	}
}

void UAxAbilitySystemTestHelper::InjectAbilityInputRelease(const UInputAction* InputAction)
{
	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(GetOwner());
	UAxAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface ? Cast<UAxAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent()) : nullptr;
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->OnInputActionEnded(InputAction);
	}
}