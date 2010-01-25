
#ifndef PRINT_STATE_H
#define PRINT_STATE_H

#include "ITickable.h"
#include "gameStateMachine.h"

class PrintState : public ITickable
{
private:
	GameStateMachine *gameStateMachine;
	
public:
	PrintState(GameStateMachine *gameStateMachine);
	void tick();
};

#endif
