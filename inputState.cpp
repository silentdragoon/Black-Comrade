
#include "inputState.h"

using namespace std;

void InputState::tick()
{
    if(mMouse)
        mMouse->capture();

    if(mKeyboard) {
        mKeyboard->capture();
        if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	        mExit->exit();
    }
}

bool InputState::isKeyDown(OIS::KeyCode keyCode) {
    if(mKeyboard)
    	return mKeyboard->isKeyDown(keyCode);
    else
    	return false;
}

void InputState::addKeyListener(OIS::KeyListener *listener) {
    if (mKeyboard) {
        mKeyboard->setEventCallback(listener);
    }
}

void InputState::clearKeyListener() {
    if (mKeyboard) mKeyboard->setEventCallback(0);
}

bool InputState::isMouseButtonDown(OIS::MouseButtonID buttonID) {
    if (mMouse)
        return mMouse->getMouseState().buttonDown(buttonID);
    else
        return false;
}

int InputState::getMouseX() {
    if (!mMouse) return 0;

    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

    int relX = mouse_state.X.rel;
    return relX;
}

int InputState::getMouseY() {
    if (!mMouse) return 0;

    const OIS::MouseState &mouse_state = mMouse->getMouseState();
    mouse_state.width = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    mouse_state.height = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

    int relY = mouse_state.Y.rel;
    return relY;
}

InputState::InputState(RenderWindow *window, bool bufferedKeys, IExit *mExit,
        	bool initKeyboard, bool initMouse) 
    : mKeyboard(0)
    , mMouse(0)
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

InputState::~InputState() {
    mInputManager->destroyInputObject( mKeyboard );
    mInputManager->destroyInputObject( mMouse );

    OIS::InputManager::destroyInputSystem(mInputManager);
    mInputManager = NULL;
}

