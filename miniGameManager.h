
#ifndef MINI_GAME_MANAGER_H
#define MINI_GAME_MANAGER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "inputState.h"
#include "IPlayerControls.h"
#include "IMiniGame.h"
#include "console.h"

using namespace Ogre;
using namespace std;

class MiniGameManager : public ITickable {
private:
    IPlayerControls *playerControls;
    InputState *inputState;
    SceneManager *sceneManager;
    Console *console;
    
    IMiniGame *currentMiniGame;
    
public:

    void tick();
    
    MiniGameManager(Console *console, InputState *inputState, IPlayerControls *playerControls, SceneManager *sceneManager);
};

#endif
