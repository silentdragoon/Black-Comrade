
#ifndef MINI_GAME_H
#define MINI_GAME_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "IConsoleInputReceiver.h"

using namespace Ogre;

class IMiniGame : public IConsoleInputReceiver {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;
    virtual int getScore() = 0;

    virtual void alphaNumKeyPressed(const OIS::KeyEvent &arg) {};
    virtual void returnKeyPressed() {};
    virtual void backspaceKeyPressed() {};
};

#endif
