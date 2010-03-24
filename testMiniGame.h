
#ifndef TEST_MINI_GAME_H
#define TEST_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"
#include "inputState.h"
#include <OIS/OIS.h>
#include "rect2D.h"
#include "console.h"

using namespace Ogre;

class TestMiniGame : public IMiniGame {
private:
    bool isEnd;
    int score;
    void createScene();
    InputState *inputState;
    Console *console;

    int x;
    int y;

public:
    void tick();
    bool end();
    int getScore();

    TestMiniGame(Console *console, InputState *inputState);

    void returnKeyPressed();

};

#endif
