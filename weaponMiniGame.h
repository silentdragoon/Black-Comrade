
#ifndef WEAPON_MINI_GAME_H
#define WEAPON_MINI_GAME_H

#include <OGRE/Ogre.h>

#include "IMiniGame.h"
#include "inputState.h"
#include <OIS/OIS.h>
#include "rect2D.h"
#include "console.h"
#include <sstream>
#include <string>

#include <boost/math/distributions/uniform.hpp>
#include <boost/random.hpp>

using namespace Ogre;

class WeaponMiniGame : public IMiniGame {
private:
    bool isEnd;
    int score;
    void createScene();
    InputState *inputState;
    Console *console;

    std::string misalignedbox;
    std::string alignedbox;

    std::string generateSequenceString();

    void generateMisalignedBox();
    void fillMisalignedBox();
    void fillAlignedBox();

    void updateKeyToPress();

    int xMisalignedStart;
    int xMisalignedEnd;
    int yMisalignedStart;
    int yMisalignedEnd;

    int xAlignedStart;
    int xAlignedEnd;
    int yAlignedStart;
    int yAlignedEnd;

    int remainingMisaligned;

    std::string sequence;
    char toPress;
    int pointerIndex;
    int pointerBase;
    int pointerY;

public:
    void tick();
    bool end();
    int getScore();

    WeaponMiniGame(Console *console, InputState *inputState);

    void alphaNumKeyPressed (const OIS::KeyEvent &arg);
    void returnKeyPressed();

};

#endif
