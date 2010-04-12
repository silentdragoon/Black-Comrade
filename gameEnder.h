#ifndef GAME_ENDER_H
#define GAME_ENDER_H

#include "ITickable.h"
#include "gameStateMachine.h"
#include "guiManager.h"
#include "IExit.h"

using namespace Ogre;

class GameEnder : public ITickable {

private:
    void tick();
    GameStateMachine *stateMachine;
    GuiManager *guiMgr;
    bool ending;
    IExit *exit;

public:
    GameEnder(GameStateMachine *stateMachine,
              GuiManager *guiMgr, IExit *exit);
};

#endif
