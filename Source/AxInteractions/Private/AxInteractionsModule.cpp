#include "AxInteractionsModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FAxInteractions"

void FAxInteractions::StartupModule()
{
}

void FAxInteractions::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAxInteractions, AxInteractions);