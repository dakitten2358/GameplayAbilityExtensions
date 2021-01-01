// (C) 2021 Jay Murray


#include "AxAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "AxGameplayAbility.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AxCommonDebug.h"

void UAxAbilitySystemComponent::BindAbilityActivationToEnhancedInputComponent(UInputComponent* InputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		CacheInputActions();

		for (const UInputAction* InputAction : CachedInputActions)
		{
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAxAbilitySystemComponent::OnInputActionStarted, InputAction);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAxAbilitySystemComponent::OnInputActionEnded, InputAction);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &UAxAbilitySystemComponent::OnInputActionEnded, InputAction);
		}
	}
}

void UAxAbilitySystemComponent::OnInputActionStarted(const UInputAction* InputAction)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability)
		{
			UAxGameplayAbility* AxGameplayAbility = Cast<UAxGameplayAbility>(Spec.Ability);
			if (AxGameplayAbility && AxGameplayAbility->AbilityInputAction == InputAction)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else if (AxGameplayAbility->bActivateAbilityOnInputAction)
				{
					// Ability is not active, so try to activate it
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

void UAxAbilitySystemComponent::OnInputActionEnded(const UInputAction* InputAction)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability)
		{
			UAxGameplayAbility* AxGameplayAbility = Cast<UAxGameplayAbility>(Spec.Ability);
			if (AxGameplayAbility && AxGameplayAbility->AbilityInputAction == InputAction)
			{
				Spec.InputPressed = false;
				if (Spec.Ability && Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputReleased(Spec.Handle);
					}

					AbilitySpecInputReleased(Spec);

					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
			}
		}
	}
}

void UAxAbilitySystemComponent::CacheInputActions()
{
	if (bHasCachedInputActions)
		return;
	
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.ClassNames = { TEXT("InputAction") };
	Filter.bRecursiveClasses = false;

	TArray<FAssetData> AssetList;
	
	// This data could be stored as static and processed in Default___CharacterClass's UObject,
	// but I think the performance hit is negligible and it's easier this way.
	AssetRegistry.GetAssets(Filter, AssetList);
	for (const FAssetData& Asset : AssetList)
	{
		UObject* Obj = Asset.GetAsset();
		const UInputAction* InputAction = Cast<UInputAction>(Obj);
		if (InputAction)
			CachedInputActions.Add(InputAction);
	}
}