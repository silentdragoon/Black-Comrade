
#include "inputState.h"

using namespace std;

void InputState::tick()
{
    mKeyboard->capture();
    mMouse->capture();

    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mExit->exit();   
}

bool InputState::isKeyDown(OIS::KeyCode keyCode)
{
    return mKeyboard->isKeyDown(keyCode);
}

int InputState::getMouseX()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    int relx = mouse_state.X.rel;
    return relx;
}

int InputState::getMouseY()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    int rely = mouse_state.Y.rel;
    return rely;
}

bool InputState::isLeftMouse()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    return mouse_state.buttonDown(OIS::MB_Left);
}

bool InputState::isRightMouse()
{
    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    return mouse_state.buttonDown(OIS::MB_Right);
}

InputState::InputState(RenderWindow *window, bool bufferedKeys, IExit *mExit) 
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
    mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeys)); 
    mMouse = static_cast<OIS::Mouse*> (mInputManager->createInputObject(OIS::OISMouse, bufferedKeys));
}

InputState::~InputState()
{
	mInputManager->destroyInputObject( mKeyboard );
    mInputManager->destroyInputObject( mMouse );

	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = NULL;
}

