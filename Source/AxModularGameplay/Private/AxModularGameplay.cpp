#include "AxModularGameplay.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FAxModularGameplay"

void FAxModularGameplay::StartupModule()
{
}

void FAxModularGameplay::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAxModularGameplay, AxModularGameplay);