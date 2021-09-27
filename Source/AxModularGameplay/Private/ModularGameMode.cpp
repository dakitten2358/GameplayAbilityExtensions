#include "ModularGameMode.h"
#include "ModularGameState.h"
#include "ModularPlayerController.h"
#include "ModularPlayerState.h"
#include "ModularPawn.h"

AModularGameModeBase::AModularGameModeBase()
{
	GameStateClass = AModularGameStateBase::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}

AModularGameMode::AModularGameMode()
{
	GameStateClass = AModularGameState::StaticClass();
	PlayerControllerClass = AModularPlayerController::StaticClass();
	PlayerStateClass = AModularPlayerState::StaticClass();
	DefaultPawnClass = AModularPawn::StaticClass();
}
