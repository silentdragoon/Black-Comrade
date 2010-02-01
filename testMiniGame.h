
#ifndef TEST_MINI_GAME_H
#define TEST_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"
#include "inputState.h"
#include "rect2D.h"

using namespace Ogre;

class TestMiniGame : public IMiniGame {
private:
    Overlay *overlay;
    bool isEnd;
    
    InputState *inputState;
public:
    void tick();
    bool end();
    Overlay *getOverlay();
    
    TestMiniGame(InputState *inputState, SceneManager *sceneManager);
    void createScene();
};

#endif
