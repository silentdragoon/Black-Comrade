
#include "weaponMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

WeaponMiniGame::WeaponMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
{
    createScene();
}

void WeaponMiniGame::createScene() {
    std::stringstream out;

    console->makeBlank();
    console->setString("Re-align the weapons matrices by repeating the following key sequence:",5,2);
    //console->setString(sequenceString,78,2);
}

void WeaponMiniGame::returnKeyPressed() {}

void WeaponMiniGame::tick() {
}

bool WeaponMiniGame::end() {
    return isEnd;
}

int WeaponMiniGame::getScore() { return score; }
