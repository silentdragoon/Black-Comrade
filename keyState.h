#ifndef KEY_STATE_H
#define KEY_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IInput.h"

class KeyState : public ITickable, public IInput
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
        virtual double forwardInput();
        virtual double sideInput();
        virtual double upInput();
        virtual double yawInput();
        virtual double pitchInput();
        
        virtual void tick();
        
        KeyState(RenderWindow *window, bool bufferedKeys = false);
        
};

#endif
