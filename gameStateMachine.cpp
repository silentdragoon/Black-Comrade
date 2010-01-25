
#include "gameStateMachine.h"

GameStateMachine::GameStateMachine(MapCreate *mapCreate, ShipState *shipState)
	: gameState(GS_STELPH)
	, mapCreate(mapCreate)
	, shipState(shipState)
{}

void GameStateMachine::tick()
{
	// TODO: Something hear
}
	
GameState GameStateMachine::currentGameState()
{
	return gameState;
}
