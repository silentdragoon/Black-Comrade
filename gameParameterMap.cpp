
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
                return 4;
                break;
			case GS_FLEE:
				return 10;
				break;
			default:
				return 0;
		}
	}
	
	// SWARMS_FIND_SHIP Parameter
	if(name.compare("SWARMS_FIND_SHIP") == 0) {
	    switch(gameStateMachine->currentGameState()) {
	        case GS_ATTACK:
	        case GS_FLEE:
	            return 1;
	            break;
	        default:
	            return 0;
	            break;
	    }
	}

	return -1;
}
