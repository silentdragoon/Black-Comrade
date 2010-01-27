
#include "gameParameterMap.h"

GameParameterMap::GameParameterMap(GameStateMachine *gameStateMachine)
	: gameStateMachine(gameStateMachine)
{}

// YOU NEED TO FREE THE MEMORY FOR THE RETURN STRING	
int GameParameterMap::getParameter(string name){

	// SPAWN Parameter
	if(name.compare("SPAWN") == 0) {
		switch(gameStateMachine->currentGameState()) {
			case GS_ATTACK:
                return 5;
                break;
			case GS_FLEE:
				return 10;
				break;
			default:
				return 0;
		}
	}

	return -1;
}
