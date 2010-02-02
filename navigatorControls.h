#ifndef _NAVIGATOR_CONTROLS_H 
#define _NAVIGATOR_CONTROLS_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <iostream>
#include "ITickable.h"
#include "inputState.h"

using namespace Ogre;
using namespace std;

class NavigatorControls : public ITickable 
{
    private:
        bool enabled;
        bool isFire;

        Camera *cam;
        InputState *inputState;
    public:
        virtual void tick();
        
        bool fire();

        NavigatorControls(InputState *inputState, Camera *cam);
        ~NavigatorControls();
};

#endif

