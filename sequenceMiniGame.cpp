
#include "sequenceMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

SequenceMiniGame::SequenceMiniGame(ShipSystem system, Console *console, InputState *inputState,
                               int difficulty)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , playing(false)
    , isComplete(false)
    , hit(false)
    , system(system)
{
    rng.seed(static_cast<unsigned int>(std::time(0)));

    setCoordinates();

    switch (difficulty) {
       case 1:
           occurences = new int[2];
           possibleChars = "ABC";
           generateSequence(2);
           break;
       case 2:
           occurences = new int[3];
           possibleChars = "ABCD";
           generateSequence(3);
           break;
       case 3:
           occurences = new int[4];
           possibleChars = "ABCDE";
           generateSequence(4);
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

std::string SequenceMiniGame::getName() {
    return "weaponGame";
}

void SequenceMiniGame::setCoordinates() {
    xMisalignedStart = 27;
    xMisalignedEnd = 38;
    yMisalignedStart = 8;
    yMisalignedEnd = 13;

    xAlignedStart = 74;
    xAlignedEnd = 85;
    yAlignedStart = 8;
    yAlignedEnd = 13;

    pointerXBase = 82;
    pointerY = 3;
}

void SequenceMiniGame::createScene() {
    console->makeBlank();
    if (system == SS_WEAPONS)
        console->setString("Re-align the weapons matrices by repeating the following key sequence:",6,2);
    else
        console->setString("Unscramble the sensor matrices by repeating the following key sequence:",6,2);
    console->setString(generateSequenceString(),79,2);
    console->setChar('^',82,3);

    console->setString("________________________                       ________________________",21,4);
    for (int i = 0 ; i < 11 ; i ++) {
        console->setString("|                        |                     |                        |",20,5+i);
    }
    console->setString("|________________________|                     |________________________|",20,16);

    console->setString("---------->",51,10);
    console->setString("---------->",51,11);

    fillMisalignedBox();
    fillAlignedBox();

    console->setString("Misaligned members remaining:",17,19);

    updateRemaining();
    console->setString("Press Return to Quit",72,19);
}


void SequenceMiniGame::generateSequence(int length) {

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

void SequenceMiniGame::calculateOccurences() {
    int mod = remainingMisaligned % sequence.length();
    for (int i = 0 ; i < sequence.length() ; i++) {
        occurences[i] = (int) floor(remainingMisaligned / sequence.length());
        if (i < mod) occurences[i] = occurences[i] + 1;
    }
}

void SequenceMiniGame::generateMisalignedBox() {
     
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

void SequenceMiniGame::updateKeyToPress() {
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

void SequenceMiniGame::updateRemaining() {
    std::stringstream out;
    out << remainingMisaligned << " ";
    console->setString(out.str(),48,19);
}

void SequenceMiniGame::fillMisalignedBox() {
    int i = 0;
    for (int y = yMisalignedStart ; y <= yMisalignedEnd ; y++) {
        for (int x = xMisalignedStart ; x <= xMisalignedEnd ; x++) {
            console->setChar(misalignedbox[i],x,y);
            i ++;
        }
    }
}

void SequenceMiniGame::fillAlignedBox() {
    int i = 0;
    for (int y = yAlignedStart ; y <= yAlignedEnd ; y++) {
        for (int x = xAlignedStart ; x <= xAlignedEnd ; x++) {
            console->setChar(alignedbox[i],x,y);
            i ++;
        }
    }
}

std::string SequenceMiniGame::generateSequenceString() {
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

void SequenceMiniGame::returnKeyPressed() {
    isEnd = true;
}

void SequenceMiniGame::moveBoxes(char c) {
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

void SequenceMiniGame::alphaNumKeyPressed (const OIS::KeyEvent &arg) {
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

void SequenceMiniGame::tick() {

    if (remainingMisaligned < totalChars * 0.9) {
        isComplete = true;
    }

    if (remainingMisaligned == 0) {
        playing = false;
        console->appendLine("Finished");
        isEnd = true;
    }
}

bool SequenceMiniGame::end() {
    return isEnd;
}

bool SequenceMiniGame::complete() { return isComplete; }

int SequenceMiniGame::getScore() {
    if (hit) {
        hit = false;
        return 1;
    } else return 0;
}

ShipSystem SequenceMiniGame::getSystem() { return system; }

