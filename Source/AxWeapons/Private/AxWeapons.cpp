// (C) 2021 Jay Murray

#include "AxWeapons.h"
#include "Logging.h"

#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FAxWeapons"

void FAxWeapons::StartupModule()
{
}

void FAxWeapons::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAxWeapons, AxWeapons);