
#ifndef GAME_PARAMETER_MAP_H
#define GAME_PARAMETER_MAP_H

#include "gameStateMachine.h"

class GameParameterMap
{
private:
	GameStateMachine *gameStateMachine;
	
public:
	GameParameterMap(GameStateMachine *gameStateMachine);
	
	int getParameter(string name);
};

#endif
