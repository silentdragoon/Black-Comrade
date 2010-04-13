
#include "weaponMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

WeaponMiniGame::WeaponMiniGame(Console *console, InputState *inputState,
                               int difficulty)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , playing(false)
    , isComplete(false)
    , hit(false)
{
    rng.seed(static_cast<unsigned int>(std::time(0)));

    setCoordinates();

    switch (difficulty) {
       case 1:
           occurences = new int[3];
           possibleChars = "ABC";
           generateSequence(3);
           break;
       case 2:
           occurences = new int[4];
           possibleChars = "ABCD";
           generateSequence(4);
           break;
       case 3:
           occurences = new int[5];
           possibleChars = "ABCDE";
           generateSequence(5);
           break;
    }

    totalChars = (xMisalignedEnd - xMisalignedStart + 1) *
                     (yMisalignedEnd - yMisalignedStart + 1);
    remainingMisaligned = totalChars;

    std::cout << "-------------------------------" << difficulty << "\n";

    calculateOccurences();

    toPress = sequence[0];
    toPressIndex = 0;

    alignedbox = std::string(totalChars,'.');
    misalignedbox = "";

    generateMisalignedBox();
    createScene();

}

std::string WeaponMiniGame::getName() { return "weaponGame"; }

void WeaponMiniGame::setCoordinates() {
    xMisalignedStart = 16;
    xMisalignedEnd = 42;
    yMisalignedStart = 6;
    yMisalignedEnd = 16;

    xAlignedStart = 70;
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

    boost::uniform_int<> six(0,possibleChars.length()-1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);

    char lastChar = 0;
    for (int x = 0 ; x < length ; x++) {
        char nextChar = possibleChars[die()];    
        while(lastChar == nextChar) {
            nextChar = possibleChars[die()];
        }
        sequence += nextChar;
        lastChar = nextChar;
    }
}

void WeaponMiniGame::calculateOccurences() {
    int mod = remainingMisaligned % sequence.length();
    for (int i = 0 ; i < sequence.length() ; i++) {
        occurences[i] = (int) floor(remainingMisaligned / sequence.length());
        if (i < mod) occurences[i] = occurences[i] + 1;
    }
}

void WeaponMiniGame::generateMisalignedBox() {
     
    boost::uniform_int<> six(0,sequence.length()-1);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);

    for (int y = yMisalignedStart ; y <= yMisalignedEnd ; y++) {
        for (int x = xMisalignedStart ; x <= xMisalignedEnd ; x++) {
            int charIndex = die();
            while (occurences[charIndex] == 0) {
                charIndex = die();
            }
            occurences[charIndex] = occurences[charIndex] - 1;
            misalignedbox += sequence[charIndex];
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
    int i = 0;
    for (int y = yAlignedStart ; y <= yAlignedEnd ; y++) {
        for (int x = xAlignedStart ; x <= xAlignedEnd ; x++) {
            console->setChar(alignedbox[i],x,y);
            i ++;
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

void WeaponMiniGame::moveBoxes(char c) {
    int first = misalignedbox.find_first_of(c);
    int last = misalignedbox.find_last_of(c);

    boost::uniform_int<> six(0,(last-first));
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);

    int index = misalignedbox.find_first_of(c,first + die());

    misalignedbox.replace(index,1,".");
    fillMisalignedBox();

    std::string str = "";
    str += c;

    alignedbox.replace(totalChars-remainingMisaligned,1,str);
    fillAlignedBox();
}

void WeaponMiniGame::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
    if (playing == false && remainingMisaligned > 0) {
        playing = true;
    }
    if (playing == false) return;
    if (arg.text == toPress || arg.text == tolower(toPress)) {
        moveBoxes(toPress);
        remainingMisaligned --;
        updateRemaining();
        updateKeyToPress();
        hit = true;
    }
}

void WeaponMiniGame::tick() {

    if (remainingMisaligned < totalChars * 0.9) {
        isComplete = true;
    }

    if (remainingMisaligned == 0) {
        playing = false;
        console->appendLine("Finished");
        isEnd = true;
    }
}

bool WeaponMiniGame::end() {
    return isEnd;
}

bool WeaponMiniGame::complete() { return isComplete; }

int WeaponMiniGame::getScore() {
    if (hit) {
        hit = false;
        return 1;
    } else return 0;
}

ShipSystem WeaponMiniGame::getSystem() { return SS_WEAPONS; }

