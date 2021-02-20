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

		TArray<uint32>& BoundActions = BoundActionsByComponent.FindOrAdd(EnhancedInputComponent);
		for (const UInputAction* InputAction : CachedInputActions)
		{
			// probably don't need Started since Triggered will get called
			//BoundActions.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UAxAbilitySystemComponent::OnInputActionStarted, InputAction).GetHandle());
			BoundActions.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &UAxAbilitySystemComponent::OnInputActionTriggered, InputAction).GetHandle());
			BoundActions.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UAxAbilitySystemComponent::OnInputActionEnded, InputAction).GetHandle());
			BoundActions.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &UAxAbilitySystemComponent::OnInputActionEnded, InputAction).GetHandle());
		}
	}
}

void UAxAbilitySystemComponent::UnbindAbilityActivateFromEnhancedInputComponent(UInputComponent* InputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		CacheInputActions();

		TArray<uint32>* BoundActionsPtr = BoundActionsByComponent.Find(EnhancedInputComponent);
		if (BoundActionsPtr)
		{
			for (uint32 handle : *BoundActionsPtr)
			{
				EnhancedInputComponent->RemoveBindingByHandle(handle);
			}
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
				UE_LOG(LogAxCommon, Warning, TEXT("found matching gameplay ability"));
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					UE_LOG(LogAxCommon, Warning, TEXT("already active"));
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
					UE_LOG(LogAxCommon, Warning, TEXT("trying to activate"));
					// Ability is not active, so try to activate it
					TryActivateAbility(Spec.Handle);
				}
			}
		}
	}
}

void UAxAbilitySystemComponent::OnInputActionTriggered(const UInputAction* InputAction)
{
	// for now, this does the same thing as started
	OnInputActionStarted(InputAction);
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

FGameplayAbilitySpecHandle UAxAbilitySystemComponent::FindAbilitySpecHandleForClass(TSubclassOf<UGameplayAbility> AbilityClass, UObject* OptionalSourceObject)
{
	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		TSubclassOf<UGameplayAbility> SpecAbilityClass = Spec.Ability->GetClass();
		if (SpecAbilityClass == AbilityClass)
		{
			if (!OptionalSourceObject || (OptionalSourceObject && Spec.SourceObject == OptionalSourceObject))
			{
				return Spec.Handle;
			}
		}
	}

	return FGameplayAbilitySpecHandle();
}

bool UAxAbilitySystemComponent::TryActivateAbilityBatched(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	bool AbilityActivated = false;
	if (InAbilityHandle.IsValid())
	{
		FScopedServerAbilityRPCBatcher GSAbilityRPCBatcher(this, InAbilityHandle);
		AbilityActivated = TryActivateAbility(InAbilityHandle, true);

		if (EndAbilityImmediately)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
			if (AbilitySpec)
			{
				UAxGameplayAbility* Ability = Cast<UAxGameplayAbility>(AbilitySpec->GetPrimaryInstance());
				Ability->ExternalEndAbility();
			}
		}

		return AbilityActivated;
	}

	return AbilityActivated;
}

void UAxAbilitySystemComponent::K2_AddLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count)
{
	AddLooseGameplayTag(GameplayTag, Count);
}

void UAxAbilitySystemComponent::K2_AddLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count)
{
	AddLooseGameplayTags(GameplayTags, Count);
}

void UAxAbilitySystemComponent::K2_RemoveLooseGameplayTag(const FGameplayTag& GameplayTag, int32 Count)
{
	RemoveLooseGameplayTag(GameplayTag, Count);
}

void UAxAbilitySystemComponent::K2_RemoveLooseGameplayTags(const FGameplayTagContainer& GameplayTags, int32 Count)
{
	RemoveLooseGameplayTags(GameplayTags, Count);
}
