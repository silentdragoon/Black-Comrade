#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include "ITickable.h"
#include "constManager.h"
#include "gameStateMachine.h"

class GameTimer : public ITickable {
private:
    GameStateMachine *gameStateMachine;
    int tickCount;
    int seconds;
    
public:
   
    void tick();

    GameTimer(GameStateMachine *gameStateMachine);

    int getTime();
};

#endif
