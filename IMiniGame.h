
#ifndef MINI_GAME_H
#define MINI_GAME_H

#include <OGRE/Ogre.h>
#include "ITickable.h"

using namespace Ogre;

class IMiniGame {
public:
    virtual void tick() = 0;
    virtual bool end() = 0;
    
    virtual Overlay *getOverlay() = 0;
};

#endif
