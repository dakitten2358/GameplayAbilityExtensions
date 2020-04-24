#include "GameplayDebuggerCategory_Attributes.h"

#if WITH_GAMEPLAY_DEBUGGER

//#define CHECK_ACTUAL_VALUES

#include "AbilitySystemComponent.h"
#include "GameFramework/Pawn.h"

FGameplayDebuggerCategory_Attributes::FGameplayDebuggerCategory_Attributes()
{
	bShowOnlyWithDebugActor = false;
	SetDataPackReplication<FRepData>(&DataPack);
}

TSharedRef<FGameplayDebuggerCategory> FGameplayDebuggerCategory_Attributes::MakeInstance()
{
	return MakeShareable(new FGameplayDebuggerCategory_Attributes());
}

void FGameplayDebuggerCategory_Attributes::FRepData::Serialize(FArchive& archive)
{
	for (auto debugString : DebugStrings)
	{
		archive << debugString;
	}
}

void FGameplayDebuggerCategory_Attributes::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	CollectAttributes(DebugActor);
	

	TArray<AActor*> OutAttachedActors;
	DebugActor->GetAttachedActors(OutAttachedActors);
	for (AActor* AttachedDebugActor : OutAttachedActors)
	{
		CollectAttributes(AttachedDebugActor);
	}

#ifdef CHECK_ACTUAL_VALUES
	DebugAttributesOn(DebugActor);
	for (AActor* AttachedDebugActor : OutAttachedActors)
	{
		DebugAttributesOn(AttachedDebugActor);
	}
#endif
}

void FGameplayDebuggerCategory_Attributes::CollectAttributes(AActor* DebugActor)
{
	UAbilitySystemComponent* AbilitySystem = DebugActor ? Cast<UAbilitySystemComponent>(DebugActor->GetComponentByClass(UAbilitySystemComponent::StaticClass())) : nullptr;
	if (AbilitySystem)
	{
		for (UAttributeSet* AttributeSet : AbilitySystem->SpawnedAttributes)
		{
			DataPack.DebugStrings.Add(DebugActor->GetName() + TEXT(".") + AttributeSet->GetName());
			CollectAttributesFromSet(AttributeSet);
		}
	}
	else if (DebugActor != nullptr)
	{
		TArray<FStringFormatArg> args;
		args.Add(DebugActor->GetName());
		DataPack.DebugStrings.Add(FString::Format(TEXT("{red}{0} has no attributes"), args));
	}
}

void FGameplayDebuggerCategory_Attributes::DebugAttributesOn(AActor* DebugActor)
{
	for (TFieldIterator<UProperty> It(DebugActor->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		UProperty* Property = *It;
		UObjectProperty* ObjectProperty = Cast<UObjectProperty>(Property);
		if (ObjectProperty == nullptr)
			continue;

		UAttributeSet* DataPtr = Cast<UAttributeSet>(ObjectProperty->GetObjectPropertyValue_InContainer(DebugActor));
		if (DataPtr == nullptr)
		{
			TArray<FStringFormatArg> args;
			args.Add(FStringFormatArg(ObjectProperty->GetName()));
			DataPack.DebugStrings.Add(FString::Format(TEXT("{red}{0} is NULL"), args));
			continue;
		}
		
		DataPack.DebugStrings.Add(TEXT("Actual Values -----"));
		CollectAttributesFromSet(DataPtr);
	}
}

void FGameplayDebuggerCategory_Attributes::CollectAttributesFromSet(UAttributeSet* AttributeSet)
{
	for (TFieldIterator<UProperty> It(AttributeSet->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		UProperty* Property = *It;
		UStructProperty* StructProperty = Cast<UStructProperty>(Property);
		check(StructProperty);
		FGameplayAttributeData* DataPtr = StructProperty->ContainerPtrToValuePtr<FGameplayAttributeData>(AttributeSet);
		check(DataPtr);

		TArray<FStringFormatArg> args;
		args.Add(FStringFormatArg(Property->GetName()));
		args.Add(FStringFormatArg(FString::SanitizeFloat(DataPtr->GetCurrentValue())));
		args.Add(FStringFormatArg(FString::SanitizeFloat(DataPtr->GetBaseValue())));

		DataPack.DebugStrings.Add(FString::Format(TEXT("\t{0}: {yellow}{1} ({2})"), args));
	}
}

void FGameplayDebuggerCategory_Attributes::OnDataPackReplicated(int32 DataPackId)
{
	MarkRenderStateDirty();
}

void FGameplayDebuggerCategory_Attributes::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	for (const FString& debugInfoString : DataPack.DebugStrings)
	{
		CanvasContext.Printf(TEXT("%s"), *debugInfoString);
	}
}




#endif