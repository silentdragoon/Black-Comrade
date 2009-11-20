
#include "keyState.h"

using namespace std;

double KeyState::forward()
{
    return mForward;
}

double KeyState::side()
{
    return mSide;
}

double KeyState::up()
{
    return mUp;
}

double KeyState::yaw()
{
    return mYaw;
}

double KeyState::pitch()
{
    return mPitch;
}

void KeyState::tick()
{
    mKeyboard->capture();
    
    //cout<< "mEnPow: " << mEnginePower;
    if(mKeyboard->isKeyDown(OIS::KC_W))
        mForward = 1.0;
   	else if(mKeyboard->isKeyDown(OIS::KC_S))
        mForward = -1.0;
   	else
   		mForward = 0.0;
    
    if(mKeyboard->isKeyDown(OIS::KC_D))
    	mSide = 1.0;
   	else if(mKeyboard->isKeyDown(OIS::KC_A))
   		mSide = -1.0;
   	else
   		mSide = 0.0;
    
    if(mKeyboard->isKeyDown(OIS::KC_SPACE))
        mUp = 1.0;
   	else if(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_LCONTROL))
   		mUp = -1.0;
   	else
   		mUp = 0.0;
  
  	if(mKeyboard->isKeyDown(OIS::KC_UP))
    	mPitch = 1.0;
   	else if(mKeyboard->isKeyDown(OIS::KC_DOWN))
   		mPitch = -1.0;
   	else
   		mPitch = 0.0;
        
    if(mKeyboard->isKeyDown(OIS::KC_LEFT))
    	mYaw = 1.0;
   	else if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
   		mYaw = -1.0;
   	else
   		mYaw = 0.0;    
         
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        mExit->exit();   
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

