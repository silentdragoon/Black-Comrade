#ifndef _ENGINEER_CONTROLS_H 
#define _ENGINEER_CONTROLS_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <iostream>
#include "ITickable.h"
#include "inputState.h"

using namespace Ogre;
using namespace std;

class EngineerControls : public ITickable 
{
    private:
        bool enabled;

        Camera *cam;
        InputState *inputState;
    public:
        virtual void tick();

        EngineerControls(InputState *inputState, Camera *cam);
        ~EngineerControls();
};

#endif

