#include "GameplayDebuggerCategory_Attributes.h"

#if WITH_GAMEPLAY_DEBUGGER

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
}

void FGameplayDebuggerCategory_Attributes::CollectAttributes(AActor* DebugActor)
{
	UAbilitySystemComponent* AbilitySystem = DebugActor ? Cast<UAbilitySystemComponent>(DebugActor->GetComponentByClass(UAbilitySystemComponent::StaticClass())) : nullptr;
	if (AbilitySystem)
	{
		for (UAttributeSet* AttributeSet : AbilitySystem->GetSpawnedAttributes())
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

void FGameplayDebuggerCategory_Attributes::CollectAttributesFromSet(UAttributeSet* AttributeSet)
{
	for (TFieldIterator<FProperty> It(AttributeSet->GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		FProperty* Property = *It;
		FStructProperty* StructProperty = CastField<FStructProperty>(Property);
		FGameplayAttributeData* DataPtr = Property->ContainerPtrToValuePtr<FGameplayAttributeData>(AttributeSet);

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