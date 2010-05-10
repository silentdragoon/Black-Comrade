
#include "gameParameterMap.h"

#include "constManager.h"

GameParameterMap::GameParameterMap(GameStateMachine *gameStateMachine)
	: gameStateMachine(gameStateMachine)
{}

// YOU NEED TO FREE THE MEMORY FOR THE RETURN STRING	
int GameParameterMap::getParameter(string name){

	// SPAWN Parameter
	if(name.compare("SPAWN") == 0) {
		switch(gameStateMachine->currentGameState()) {
			case GS_BLACK_COMRADE:
			case GS_ATTACK:
                return ConstManager::getInt("max_swarms_attack");
                break;
			case GS_FLEE:
				return ConstManager::getInt("max_swarms_flee");
				break;
			default:
				return 0;
		}
	}
	
	// SPAWN Parameter
	if(name.compare("PATROL_SPEED") == 0) {
		switch(gameStateMachine->currentGameState()) {
			case GS_STEALTH:
                return ConstManager::getInt("enemy_patrol_stealth_speed")
                    * ConstManager::getFloat("tick_period");
                break;
			default:
				return ConstManager::getInt("enemy_patrol_attack_speed")
				    * ConstManager::getFloat("tick_period");
		}
	}
	
	// SWARMS_FIND_SHIP Parameter
	if(name.compare("SWARMS_FIND_SHIP") == 0) {
	    switch(gameStateMachine->currentGameState()) {
	        case GS_ATTACK:
            case GS_BLACK_COMRADE:
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
