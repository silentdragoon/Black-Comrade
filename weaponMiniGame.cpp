
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
    sequence[0] = 'A';
    sequence[1] = 'B';
    sequence[2] = 'C';
    sequence[3] = '\0';
    createScene();
}

void WeaponMiniGame::createScene() {
    console->makeBlank();
    console->setString("Re-align the weapons matrices by repeating the following key sequence:",6,2);
    console->setString(generateSequenceString(),79,2);

    console->setString("_______________________________                       _______________________________",14,4);
    for (int i = 0 ; i < 12 ; i ++) {
        console->setString("|                               |                     |                               |",13,5+i);
    }
    console->setString("|_______________________________|                     |_______________________________|",13,17);

    console->setString("---------->",51,10);
    console->setString("---------->",51,11);

    console->setString("Misaligned members remaining:",12,20);
    console->setString("100",43,20);
    console->setString("Press F2 to Quit",75,20);
}

std::string WeaponMiniGame::generateSequenceString() {
    std::stringstream out;
    out << "[";
    for (int i = 0 ; i < strlen(sequence); i++) {
        out << "  ";
        out << sequence[i];
        out << "  ";
    }
    out << "]";
    return out.str();
}

void WeaponMiniGame::returnKeyPressed() {}

void WeaponMiniGame::tick() {
}

bool WeaponMiniGame::end() {
    return isEnd;
}

int WeaponMiniGame::getScore() { return score; }
