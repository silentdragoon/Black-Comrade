
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
    console->setChar('*',console->getWidth(), console->getHeight());
    //console->setString("Start",5,4);
}

void TestMiniGame::returnKeyPressed() {}

void TestMiniGame::tick() {
    if (inputState->isKeyDown(OIS::KC_0)) isEnd = true;
}

bool TestMiniGame::end() {
    return isEnd;
}

int TestMiniGame::getScore() { return score; }
