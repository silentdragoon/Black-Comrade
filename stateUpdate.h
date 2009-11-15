
#ifndef STATE_UPDATE_H
#define STATE_UPDATE_H

#define TICK_PERIOD 0.01

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include <vector>

#include "shipState.h"
#include "IMotionState.h"
#include "IAccelerationState.h"


using namespace Ogre;
using namespace std;

class StateUpdate : public FrameListener{
private:
    Real timeSinceLastEvent;
    int count;
    
    vector <ITickable*> tickables;
    
    void tick();
   
public:
    
    StateUpdate();
    
    void addTickable(ITickable* t); 
    
    bool frameStarted (const FrameEvent &evt) {}
    bool frameRenderingQueued (const FrameEvent &evt);
    bool frameEnded (const FrameEvent &evt) {}
};

#endif
