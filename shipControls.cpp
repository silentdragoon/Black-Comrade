
#include "shipControls.h"

using namespace std;

double ShipControls::forward()
{
    return mForward;
}

double ShipControls::side()
{
    return mSide;
}

double ShipControls::up()
{
    return mUp;
}

double ShipControls::yaw()
{
    return mYaw;
}

double ShipControls::pitch()
{
    return mPitch;
}

bool ShipControls::fire()
{
    return isFire;
}

void ShipControls::setEnabled(bool b)
{
    enabled = b;
}

void ShipControls::tick()
{
    if(enabled) {
        if(keyState->isKeyDown(OIS::KC_W))
            mForward = 1.0;
       	else if(keyState->isKeyDown(OIS::KC_S))
            mForward = -1.0;
       	else
       		mForward = 0.0;
        
        if(keyState->isKeyDown(OIS::KC_D))
        	mSide = 1.0;
       	else if(keyState->isKeyDown(OIS::KC_A))
       		mSide = -1.0;
       	else
       		mSide = 0.0;
        
        if(keyState->isKeyDown(OIS::KC_SPACE))
            mUp = 1.0;
       	else if(keyState->isKeyDown(OIS::KC_LSHIFT) || keyState->isKeyDown(OIS::KC_LCONTROL))
       		mUp = -1.0;
       	else
       		mUp = 0.0;
      
      	if(keyState->isKeyDown(OIS::KC_UP))
        	mPitch = 1.0;
       	else if(keyState->isKeyDown(OIS::KC_DOWN))
       		mPitch = -1.0;
       	else
       		mPitch = 0.0;
            
        if(keyState->isKeyDown(OIS::KC_LEFT))
        	mYaw = 1.0;
       	else if(keyState->isKeyDown(OIS::KC_RIGHT))
       		mYaw = -1.0;
       	else
       		mYaw = 0.0;
       		
        isFire = keyState->isKeyDown(OIS::KC_RETURN);
    }
}

ShipControls::ShipControls(KeyState *keyState)
    : keyState(keyState)
    , enabled(true)
{}

ShipControls::~ShipControls() {}
