
#include "weaponMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

WeaponMiniGame::WeaponMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , playing(false)
{
    possibleChars = "ABC";
    alignedbox = "";
    misalignedbox = "";

    setCoordinates();

    generateSequence(3);
    remainingMisaligned = -1;
    toPress = sequence[0];
    toPressIndex = 0;

    generateMisalignedBox();
    createScene();
}

void WeaponMiniGame::setCoordinates() {
    xMisalignedStart = 16;
    xMisalignedEnd = 42;
    yMisalignedStart = 6;
    yMisalignedEnd = 16;

    xAlignedStart = 69;
    xAlignedEnd = 96;
    yAlignedStart = 6;
    yAlignedEnd = 16;

    pointerXBase = 82;
    pointerY = 3;

}

void WeaponMiniGame::createScene() {
    console->makeBlank();
    console->setString("Re-align the weapons matrices by repeating the following key sequence:",6,2);
    console->setString(generateSequenceString(),79,2);
    console->setChar('^',82,3);

    console->setString("_______________________________                       _______________________________",14,4);
    for (int i = 0 ; i < 12 ; i ++) {
        console->setString("|                               |                     |                               |",13,5+i);
    }
    console->setString("|_______________________________|                     |_______________________________|",13,17);

    console->setString("---------->",51,10);
    console->setString("---------->",51,11);

    fillMisalignedBox();
    fillAlignedBox();

    console->setString("Misaligned members remaining:",12,20);
    updateRemaining();
    console->setString("Press Return to Quit",73,20);
}


void WeaponMiniGame::generateSequence(int length) {
    boost::mt19937 rng;                 
    boost::uniform_int<> six(0,possibleChars.length()-1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);
    sequence = "ABC";
}

void WeaponMiniGame::generateMisalignedBox() {

    boost::mt19937 rng;                 
    boost::uniform_int<> six(0,sequence.length()-1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);

    remainingMisaligned = 0;
    for (int y = yMisalignedStart ; y <= yMisalignedEnd ; y++) {
        for (int x = xMisalignedStart ; x <= xMisalignedEnd ; x++) {
            misalignedbox += sequence[die()];
            remainingMisaligned ++;
        }
    }
}

void WeaponMiniGame::updateKeyToPress() {
    if (toPressIndex + 1 == sequence.length()) {
        toPressIndex = 0;
    } else {
        toPressIndex ++;
    }

    for (int i = 0 ; i < sequence.length() ; i++) {
        if (i == toPressIndex) {
            console->setChar('^',pointerXBase + (i*5),pointerY);
        } else {
            console->setChar(' ',pointerXBase + (i*5),pointerY);
        }
    }

    toPress = sequence[toPressIndex];
    
}

void WeaponMiniGame::updateRemaining() {
    std::stringstream out;
    out << remainingMisaligned << " ";
    console->setString(out.str(),43,20);
}

void WeaponMiniGame::fillMisalignedBox() {
    int i = 0;
    for (int y = yMisalignedStart ; y <= yMisalignedEnd ; y++) {
        for (int x = xMisalignedStart ; x <= xMisalignedEnd ; x++) {
            console->setChar(misalignedbox[i],x,y);
            i ++;
        }
    }
}

void WeaponMiniGame::fillAlignedBox() {
    for (int y = yAlignedStart ; y <= yAlignedEnd ; y++) {
        for (int x = xAlignedStart ; x <= xAlignedEnd ; x++) {
            console->setChar('.',x,y);
        }
    }
}

std::string WeaponMiniGame::generateSequenceString() {
    std::stringstream out;
    out << "[";
    for (int i = 0 ; i < sequence.length(); i++) {
        out << "  ";
        out << sequence[i];
        out << "  ";
    }
    out << "]";
    return out.str();
}

void WeaponMiniGame::returnKeyPressed() {
    isEnd = true;
}

void WeaponMiniGame::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
    if (playing == false && remainingMisaligned > 0) playing = true;
    if (playing == false) return;
    if (arg.text == toPress || arg.text == tolower(toPress)) {
        updateKeyToPress();
        remainingMisaligned --;
        updateRemaining();
    }
}

void WeaponMiniGame::tick() {
    if (remainingMisaligned == 0) {
        playing = false;
        console->appendLine("Finished");
        isEnd = true;
    }
}

bool WeaponMiniGame::end() {
    return isEnd;
}

int WeaponMiniGame::getScore() { return score; }
