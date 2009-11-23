
#include "keyState.h"

using namespace std;

void KeyState::tick()
{
    mKeyboard->capture();
    
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mExit->exit();   
}

bool KeyState::isKeyDown(OIS::KeyCode keyCode)
{
    return mKeyboard->isKeyDown(keyCode);
}

KeyState::KeyState(RenderWindow *window, bool bufferedKeys, IExit *mExit) 
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

KeyState::~KeyState()
{
	mInputManager->destroyInputObject( mKeyboard );

	OIS::InputManager::destroyInputSystem(mInputManager);
	mInputManager = NULL;
}

