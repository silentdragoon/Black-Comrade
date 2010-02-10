
#ifndef STATE_UPDATE_H
#define STATE_UPDATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include <vector>

#include "shipState.h"
#include "IMotionState.h"
#include "IAccelerationState.h"
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class StateUpdate : public FrameListener{
private:
    Real timeSinceLastEvent;
    int count;
    
    std::vector <ITickable*> tickables;
   
public:
    
    bool running;
    
    StateUpdate();
    
    void addTickable(ITickable* t); 
    
    bool frameStarted (const FrameEvent &evt) {}
    bool frameRenderingQueued (const FrameEvent &evt);
    bool frameEnded (const FrameEvent &evt) {}
    
    // You can call this manually if thats the way you roll
    bool tick();
    
    void startLoop();
};

#endif
