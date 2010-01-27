
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapManager *mapManager, ShipState *shipState)
	: gameState(GS_STELPH)
	, mapManager(mapManager)
	, shipState(shipState)
	, mIsNewState(true)
{}

void GameStateMachine::tick()
{
	GameState oldState = gameState;
	mIsNewState = false;

	// Waypoint events
	string *wp = mapManager->getWaypoint(shipState->position);
	if(wp != NULL) {
		if(*wp == "wp_attack") {
			switch(gameState) {
				case GS_STELPH:
					gameState = GS_ATTACK;
					break;
			}
		}
	}
	
	if(oldState != gameState) mIsNewState = true;
}
	
GameState GameStateMachine::currentGameState()
{
	return gameState;
}

bool GameStateMachine::isNewState()
{
	return mIsNewState;
}
