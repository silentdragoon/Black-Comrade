
#ifndef SHIP_CONTROLS_H
#define SHIP_CONTROLS_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IAccelerationState.h"
#include "inputState.h"

class ShipControls : public ITickable, public IAccelerationState
{
    private:
        double mForward;
        double mSide;
        double mUp;
        double mYaw;
        double mPitch;
        
        bool isFire;
        bool enabled;
        
        InputState *inputState;
        
    public:    
        virtual double forward();
        virtual double side();
        virtual double up();
        virtual double yaw();
        virtual double pitch();
        bool fire();
        
        void setEnabled(bool b);
        
        virtual void tick();
        
        ShipControls(InputState *inputState);
        ~ShipControls();
};

#endif
