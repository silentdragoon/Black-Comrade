
#include "testMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

TestMiniGame::TestMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
{
    createScene();
}

void TestMiniGame::createScene() {
    console->makeBlank();
    for (int y = 0; y < console->getHeight(); y++) {
        for (int x = 0; x < console->getWidth(); x++) {
            console->setChar('#',x,y);
        }
    }
}

void TestMiniGame::returnKeyPressed() {}

void TestMiniGame::tick() {
    if (inputState->isKeyDown(OIS::KC_0)) isEnd = true;
}

bool TestMiniGame::end() {
    return isEnd;
}

int TestMiniGame::getScore() { return score; }
