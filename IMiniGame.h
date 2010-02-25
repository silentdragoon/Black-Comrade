
#ifndef MINI_GAME_H
#define MINI_GAME_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "ITickable.h"

using namespace Ogre;

class IMiniGame {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;
    virtual int getScore() = 0;

    virtual void keyPressed(OIS::KeyEvent &arg) {};
    virtual void keyReleased(OIS::KeyEvent &arg) {};
};

#endif
