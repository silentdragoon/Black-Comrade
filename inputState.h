
#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IExit.h"

using namespace Ogre;

class InputState : public ITickable
{
    private:
        OIS::Keyboard* mKeyboard;
        OIS::Mouse* mMouse;
        OIS::InputManager* mInputManager;
        RenderWindow *mWindow;
        
        IExit *mExit;
        
    public:    
        bool isKeyDown(OIS::KeyCode keyCode);
        bool isMouseMoved(const OIS::MouseEvent &e);

        int getMouseX();
        int getMouseY();
        
        virtual void tick();
        
        InputState(RenderWindow *window, bool bufferedKeys, IExit *mExit);
        ~InputState();
};

#endif
