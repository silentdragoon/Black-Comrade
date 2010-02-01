
#include "inputState.h"

using namespace std;

void InputState::tick()
{
    mKeyboard->capture();
    
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mExit->exit();   
}

bool InputState::isKeyDown(OIS::KeyCode keyCode)
{
    return mKeyboard->isKeyDown(keyCode);
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
}

InputState::~InputState()
{
	mInputManager->destroyInputObject( mKeyboard );

	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = NULL;
}

