
#include "stateUpdate.h"
#include "const.h"

#include <iostream>

StateUpdate::StateUpdate() 
    : timeSinceLastEvent(0)
    , count(0)
    , running(true)
    ,latestSlack(0)
{}

bool StateUpdate::frameRenderingQueued (const FrameEvent &evt)
{   
    if(running) {

        timeSinceLastEvent += evt.timeSinceLastFrame;
        
        if(timeSinceLastEvent > TICK_PERIOD) {
        	std::cout << (timeSinceLastEvent - TICK_PERIOD) << std::endl;
            timeSinceLastEvent = 0;
            tick();
        }
    }
    
    return running;
}

void StateUpdate::startLoop()
{
	WindowEventUtilities weu = WindowEventUtilities();
    Root *root = Root::getSingletonPtr();
    
    Timer timer;
    
    long oldtime;
    long newtime;
    long looptime;
    long sleep;
    
    bool loop = true;
    while(loop) {
        //std::cout << timer.getMilliseconds() - oldtime << std::endl;
    	oldtime = timer.getMilliseconds();
    	loop = this->tick();
    	weu.messagePump();
    	root->renderOneFrame();
    	newtime = timer.getMilliseconds();
    	looptime = newtime - oldtime;
    	sleep = (long)(1000 * TICK_PERIOD) - looptime;
    	//sleep = (sleep > 0) ? sleep : 0;
    	latestSlack = sleep;
    	while(timer.getMilliseconds() < newtime + sleep);
    }
}

void StateUpdate::addTickable(ITickable* t)
{
    tickables.push_back(t);
}

long StateUpdate::getSlack()
{
    return latestSlack;
}

// Called once every TICK_PERIOD seconds
bool StateUpdate::tick() 
{


    // std::cout << "Tick " << ++count << std::endl;
    std::vector<ITickable*>::iterator i;
    ITickable *t;

    
    
    for(i = tickables.begin(); i != tickables.end(); ++i) {
        if (running == false) break;
        //std::cout << "Tick " << ++count << std::endl;
        t = *i;
        t->tick();
    }

	return running;
}
