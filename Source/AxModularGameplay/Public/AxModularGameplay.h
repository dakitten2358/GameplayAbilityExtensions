#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAxModularGameplay : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	void StartupModule() override;
	void ShutdownModule() override;
	
};
