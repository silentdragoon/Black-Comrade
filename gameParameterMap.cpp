
#include "gameParameterMap.h"

GameParameterMap::GameParameterMap(GameStateMachine *gameStateMachine)
	: gameStateMachine(gameStateMachine)
{}

// YOU NEED TO FREE THE MEMORY FOR THE RETURN STRING	
string* GameParameterMap::getParameter(string name){

	// SPAWN Parameter
	if(name.compare("SPAWN") == 0) {
		switch(gameStateMachine->currentGameState()) {
			case GS_ATTACK:
			case GS_FLEE:
				return new string("Y");
				break;
			default:
				return new string("N");
		}
	}

	return NULL;
}
