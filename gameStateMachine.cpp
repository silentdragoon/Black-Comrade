
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

	// TODO: Something hear
	string *wp = mapCreate->getWaypoint(shipState->position);
	if(wp != NULL) {
		//cout << wp << "\n";
		if(wp->compare("wp_attack") == 0) {
			switch(gameState) {
				case GS_STELPH:
					gameState = GS_ATTACK;
					std::cout << "Change!\n";
					break;
			}
		}
	}
	delete wp;
	
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
