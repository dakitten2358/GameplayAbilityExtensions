// (C) 2021 Jay Murray

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAxWeapons : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
	
};
