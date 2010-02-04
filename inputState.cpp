
#include "inputState.h"

using namespace std;

void InputState::tick()
{
	if(mMouse)
		mMouse->capture();

	if(mKeyboard)
	{
	    mKeyboard->capture();
	    

	    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	        mExit->exit();
    }
}

bool InputState::isKeyDown(OIS::KeyCode keyCode)
{
	if(mKeyboard)
    	return mKeyboard->isKeyDown(keyCode);
    else
    	return false;
}

bool InputState::isMouseButtonDown(OIS::MouseButtonID buttonID) {
    return mMouse->getMouseState().buttonDown(buttonID);
}

int InputState::getMouseX()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    //int relx = mMouse->getMouseState().X.rel;
    int relx = mouse_state.X.rel;
    //if(relx>25) relx=25;
    //if(relx<-25) relx=-25;
    if(mMouse) return relx;
    else return 0;

}

int InputState::getMouseY()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    //int rely = mMouse->getMouseState().Y.rel;
    int rely = mouse_state.Y.rel;
    //if(rely>25) rely=25;
    //if(rely<-25) rely=-25;
    if(mMouse) return rely;
	else return 0;
}

InputState::InputState(RenderWindow *window, bool bufferedKeys, IExit *mExit,
        	bool initKeyboard, bool initMouse) 
    : mKeyboard(0)
    , mWindow(mWindow)
    , mExit(mExit)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);
    
    if(initKeyboard) {
    	mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeys)); 
    }
    if(initMouse) {
    	mMouse = static_cast<OIS::Mouse*> (mInputManager->createInputObject(OIS::OISMouse, bufferedKeys));
    }
}

InputState::~InputState()
{
    mInputManager->destroyInputObject( mKeyboard );
    mInputManager->destroyInputObject( mMouse );

    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = NULL;
}

