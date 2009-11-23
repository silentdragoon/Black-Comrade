
#ifndef KEY_STATE_H
#define KEY_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IExit.h"

using namespace Ogre;

class KeyState : public ITickable
{
    private:
        OIS::Keyboard* mKeyboard;
        OIS::InputManager* mInputManager;
        RenderWindow *mWindow;
        
        IExit *mExit;
        
    public:    
        bool isKeyDown(OIS::KeyCode keyCode);
        
        virtual void tick();
        
        KeyState(RenderWindow *window, bool bufferedKeys, IExit *mExit);
        ~KeyState();
};

#endif
