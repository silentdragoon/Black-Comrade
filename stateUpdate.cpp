
#include "stateUpdate.h"

#include <iostream>

StateUpdate::StateUpdate() :
    timeSinceLastEvent(0),
    count(0)
{}

bool StateUpdate::frameRenderingQueued (const FrameEvent &evt)
{   
    timeSinceLastEvent += evt.timeSinceLastFrame;
    
    if(timeSinceLastEvent > TICK_PERIOD) {
        timeSinceLastEvent = 0;
        
        tick();
    }
    
    return true;
}


// Called once every TICK_PERIOD seconds
void StateUpdate::tick() 
{
    std::cout << "Tick " << ++count << std::endl;
}
