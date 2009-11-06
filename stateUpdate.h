
#ifndef STATE_UPDATE_H
#define STATE_UPDATE_H

#define TICK_PERIOD 0.01

// Includes all Ogre classes
#include <OGRE/Ogre.h>

using namespace Ogre;

class StateUpdate : public FrameListener{
private:
    Real timeSinceLastEvent;
    int count;
public:
    
    StateUpdate();
    
    void tick();
    
    bool frameStarted (const FrameEvent &evt) {}
    bool frameRenderingQueued (const FrameEvent &evt);
    bool frameEnded (const FrameEvent &evt) {}
};

#endif
