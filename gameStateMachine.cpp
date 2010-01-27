
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapCreate *mapCreate, ShipState *shipState)
	: gameState(GS_STELPH)
	, mapCreate(mapCreate)
	, shipState(shipState)
	, mIsNewState(true)
{}

void GameStateMachine::tick()
{
	GameState oldState = gameState;
	mIsNewState = false;

	// Waypoint events
	string *wp = mapCreate->getWaypoint(shipState->position);
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
