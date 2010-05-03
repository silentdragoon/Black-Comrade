
#ifndef SEQUENCE_MINI_GAME_H
#define SEQUENCE_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"
#include "inputState.h"
#include <OIS/OIS.h>
#include "rect2D.h"
#include "console.h"
#include "shipSystem.h"

#include <sstream>
#include <string>
#include <math.h>

#include <boost/math/distributions/uniform.hpp>
#include <boost/random.hpp>

using namespace Ogre;

class SequenceMiniGame : public IMiniGame {
private:

    boost::mt19937 rng;

    bool playing;
    bool isEnd;
    bool isComplete;

    ShipSystem system;
    bool hit;
    void createScene();
    InputState *inputState;
    Console *console;

    std::string misalignedbox;
    std::string alignedbox;
    int* occurences;

    std::string generateSequenceString();

    void setCoordinates();

    void generateSequence(int length);
    void calculateOccurences();
    void generateMisalignedBox();
    void fillMisalignedBox();
    void fillAlignedBox();

    void updateKeyToPress();
    void updateRemaining();

    void moveBoxes(char c);

    int xMisalignedStart;
    int xMisalignedEnd;
    int yMisalignedStart;
    int yMisalignedEnd;

    int xAlignedStart;
    int xAlignedEnd;
    int yAlignedStart;
    int yAlignedEnd;

    int totalChars;
    int remainingMisaligned;

    std::string possibleChars;
    std::string sequence;
    char toPress;
    int toPressIndex;
    int pointerXBase;
    int pointerY;

public:
    std::string getName();
    void tick();
    bool end();
    bool complete();
    int getScore();
    ShipSystem getSystem();

    SequenceMiniGame(ShipSystem system, Console *console, InputState *inputState, int difficulty);

    void alphaNumKeyPressed (const OIS::KeyEvent &arg);
    void returnKeyPressed();

};

#endif
