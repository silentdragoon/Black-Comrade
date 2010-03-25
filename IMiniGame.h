
#ifndef MINI_GAME_H
#define MINI_GAME_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include "shipSystem.h"
#include "IConsoleInputReceiver.h"

using namespace Ogre;

class IMiniGame : public IConsoleInputReceiver {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;
    virtual int getScore() = 0;
    virtual ShipSystem getSystem() { return SS_NONE;} ;
    virtual bool complete() { return false;} ;            // If you want difficulty to increase for the next mini-game,
                                                          //make sure this returns true when the game  exits
};

#endif
