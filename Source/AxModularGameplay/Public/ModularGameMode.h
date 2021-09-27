#pragma once

#include "GameFramework/GameMode.h"
#include "ModularGameMode.generated.h"

/** Pair this with a ModularGameStateBase */
UCLASS(Blueprintable)
class AXMODULARGAMEPLAY_API AModularGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AModularGameModeBase();
};

/** Pair this with a ModularGameState */
UCLASS(Blueprintable)
class AXMODULARGAMEPLAY_API AModularGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AModularGameMode();
};