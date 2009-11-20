
#include "stateUpdate.h"

#include <iostream>

StateUpdate::StateUpdate() 
    : timeSinceLastEvent(0)
    , count(0)
    , running(true)
{}

bool StateUpdate::frameRenderingQueued (const FrameEvent &evt)
{   
    if(running) {
        timeSinceLastEvent += evt.timeSinceLastFrame;
        
        if(timeSinceLastEvent > TICK_PERIOD) {
            timeSinceLastEvent = 0;
            
            tick();
        }
    }
    
    return running;
}

void StateUpdate::addTickable(ITickable* t)
{
    tickables.push_back(t);
}

// Called once every TICK_PERIOD seconds
void StateUpdate::tick() 
{
    //std::cout << "Tick " << ++count << std::endl;
    
    vector<ITickable*>::iterator i;
    ITickable *t;
    
    
    for(i = tickables.begin(); i != tickables.end(); ++i) {
    
        t = *i;
        t->tick();
    }
}
