#ifndef _NAVIGATOR_CONTROLS_H 
#define _NAVIGATOR_CONTROLS_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <iostream>
#include "ITickable.h"
#include "IPlayerControls.h"
#include "inputState.h"

using namespace Ogre;
using namespace std;

class NavigatorControls : public ITickable, public IPlayerControls
{
    private:
        bool enabled;
        bool isFire;
        bool isFullMap;

        Camera *cam;
        InputState *inputState;
    public:
        virtual void tick();
        virtual bool fire();
        virtual bool isMap();

        virtual Vector3 cameraPosition();
        virtual Quaternion cameraOrientation();

        NavigatorControls(InputState *inputState, Camera *cam);
        ~NavigatorControls();
};

#endif

