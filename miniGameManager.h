
#ifndef MINI_GAME_MANAGER_H
#define MINI_GAME_MANAGER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "inputState.h"
#include "shipControls.h"
#include "IMiniGame.h"

using namespace Ogre;
using namespace std;

class MiniGameManager : public ITickable {
private:
    ShipControls *shipControls;
    InputState *inputState;
    SceneManager *sceneManager;
    
    IMiniGame *currentMiniGame;
    
public:

    void tick();
    
    MiniGameManager(InputState *inputState, ShipControls *shipControls, SceneManager *sceneManager);
};

#endif
