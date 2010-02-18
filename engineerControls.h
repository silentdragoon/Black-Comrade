#ifndef _ENGINEER_CONTROLS_H 
#define _ENGINEER_CONTROLS_H

#include <OGRE/Ogre.h>
#include <OIS/OIS.h>
#include <iostream>
#include "ITickable.h"
#include "inputState.h"
#include "IPlayerControls.h"

using namespace Ogre;
using namespace std;

class EngineerControls : public ITickable, public IPlayerControls
{
    private:
        bool enabled;
        bool isFire;
        bool isChangeShield;
        bool isChangeWeapons;
        bool isChangeSensors;

        Camera *cam;
        InputState *inputState;
    public:
        virtual void tick();
        virtual bool fire();
        virtual bool isShield();
        virtual bool isWeapons();
        virtual bool isSensors();

        virtual Vector3 cameraPosition();
        virtual Quaternion cameraOrientation();

        EngineerControls(InputState *inputState, Camera *cam);
        ~EngineerControls();
};

#endif

