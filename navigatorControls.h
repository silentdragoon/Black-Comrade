#ifndef _NAVIGATOR_CONTROLS_H 
#define _NAVIGATOR_CONTROLS_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <iostream>
#include "ITickable.h"
#include "gunnerControls.h"
#include "inputState.h"

using namespace Ogre;
using namespace std;

class NavigatorControls : public ITickable, public GunnerControls
{
    private:
        bool isFullMap;

        InputState *inputState;
    public:
        virtual void tick();
       
        virtual bool isMap();

        NavigatorControls(InputState *inputState, Camera *cam);
        ~NavigatorControls();
};

#endif

