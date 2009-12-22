
#ifndef TEST_MINI_GAME_H
#define TEST_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"
#include "keyState.h"

using namespace Ogre;

class TestMiniGame : public IMiniGame {
private:
    Overlay *overlay;
    bool isEnd;
    
    KeyState *keyState;
public:
    void tick();
    bool end();
    Overlay *getOverlay();
    
    TestMiniGame(KeyState *keyState);
};

#endif
