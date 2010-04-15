
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
        
    public:
        bool isKeyDown(OIS::KeyCode keyCode);
        bool isMouseMoved(const OIS::MouseEvent &e);

        bool isMouseButtonDown(OIS::MouseButtonID buttonID);
        int getMouseX();
        int getMouseY();

        void releaseMouse();
        void releaseKeyboard();

        void addKeyListener(OIS::KeyListener *listener);
        void addMouseListener(OIS::MouseListener *listener);
        void clearKeyListener();
        void clearMouseListener();

        
        virtual void tick();
        
        InputState(RenderWindow *window, bool bufferedKeys,
        	   bool initKeyboard, bool initMouse);
        ~InputState();
};

#endif
