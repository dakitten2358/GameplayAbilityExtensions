// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AxCommonModule.h"

#if WITH_GAMEPLAY_DEBUGGER
#include "GameplayDebugger.h"
#include "GameplayDebuggerCategory_Attributes.h"
#endif


#define LOCTEXT_NAMESPACE "FAxCommonModule"

void FAxCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if WITH_GAMEPLAY_DEBUGGER
	IGameplayDebugger& GameplayDebuggerModule = IGameplayDebugger::Get();
	GameplayDebuggerModule.RegisterCategory("Attributes", IGameplayDebugger::FOnGetCategory::CreateStatic(&FGameplayDebuggerCategory_Attributes::MakeInstance));
	GameplayDebuggerModule.NotifyCategoriesChanged();
#endif
}

void FAxCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAxCommonModule, AxCommon)