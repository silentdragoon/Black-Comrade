
#ifndef TEST_MINI_GAME_H
#define TEST_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"

class TestMiniGame : public IMiniGame {
private:
    Overlay *overlay;
    bool isEnd;
public:
    void tick();
    bool end();
    Overlay *getOverlay();
    
    TestMiniGame();
};

#endif
