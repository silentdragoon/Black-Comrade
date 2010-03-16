
#ifndef SHIP_CONTROLS_H
#define SHIP_CONTROLS_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IAccelerationState.h"
#include "inputState.h"
#include "gunnerControls.h"

class PilotControls : public ITickable, public IAccelerationState, public GunnerControls
{
    private:

        double mForward;
        double mSide;
        double mUp;
        double mYaw;
        double mPitch;
        
        InputState *inputState;
        
    public:    
        virtual double forward();
        virtual double side();
        virtual double up();
        virtual double yaw();
        virtual double pitch();
        
        virtual void tick();
        
        PilotControls(InputState *inputState, Camera *cam);
        ~PilotControls();
};

#endif
