#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class UAbilitySystemComponent;

class FGameplayDebuggerCategory_Attributes : public FGameplayDebuggerCategory
{
public:
	FGameplayDebuggerCategory_Attributes();

	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void OnDataPackReplicated(int32 DataPackId) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

protected:
	struct FRepData
	{
		TArray<FString> DebugStrings;
		void Serialize(FArchive& archive);
	};
	FRepData DataPack;

	void CollectAttributes(AActor* DebugActor);
	void CollectAttributesFromSet(class UAttributeSet* AttributeSet);
};

#endif