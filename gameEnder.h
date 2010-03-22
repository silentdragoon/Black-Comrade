#ifndef GAME_ENDER_H
#define GAME_ENDER_H

#include "ITickable.h"
#include "gameStateMachine.h"
#include "IExit.h"

using namespace Ogre;

class GameEnder : public ITickable {

private:
    void tick();
    GameStateMachine *stateMachine;
    IExit *exit;

public:
    GameEnder(GameStateMachine *stateMachine,IExit *exit);
};

#endif
