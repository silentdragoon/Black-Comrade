
#ifndef KEY_STATE_H
#define KEY_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IAccelerationState.h"

using namespace Ogre;

class KeyState : public ITickable, public IAccelerationState
{
    private:
        double mForward;
        double mSide;
        double mUp;
        double mYaw;
        double mPitch;
        
        OIS::Keyboard* mKeyboard;
        OIS::InputManager* mInputManager;
        
    public:    
        virtual double forward();
        virtual double side();
        virtual double up();
        virtual double yaw();
        virtual double pitch();
        
        virtual void tick();
        
        KeyState(RenderWindow *window, bool bufferedKeys = false);
        
};

#endif
