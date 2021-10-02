#include "Components/AxGrantAbilitiesComponent.h"
#include "AbilitySystemGlobals.h"

UAxGrantAbilitiesComponent::UAxGrantAbilitiesComponent()
{
	
}

void UAxGrantAbilitiesComponent::GrantAbilities(AActor* ToActor, int Level)
{
	UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ToActor);
	if (AbilitySystem != nullptr && AbilitySet != nullptr && !GrantedAbilitySetHandles.Contains(ToActor))
	{
		FAxGameplayAbilitySetHandles GrantedAbilityHandles;
		AbilitySet->Give(ToActor, this, Level, GrantedAbilityHandles);
		GrantedAbilitySetHandles.Add(ToActor, GrantedAbilityHandles);
	}
}

void UAxGrantAbilitiesComponent::RemoveAbilities(AActor* FromActor)
{
	if (GrantedAbilitySetHandles.Contains(FromActor))
	{
		FAxGameplayAbilitySetHandles GrantedAbilityHandles = GrantedAbilitySetHandles[FromActor];
		AbilitySet->Remove(FromActor, GrantedAbilityHandles);
		GrantedAbilitySetHandles.Remove(FromActor);
	}
}