
#include "pilotControls.h"

using namespace std;

double PilotControls::forward()
{
    return mForward;
}

double PilotControls::side()
{
    return mSide;
}

double PilotControls::up()
{
    return mUp;
}

double PilotControls::yaw()
{
    return mYaw;
}

double PilotControls::pitch()
{
    return mPitch;
}

bool PilotControls::fire()
{
    return isFire;
}

Vector3 PilotControls::cameraPosition() {
    return cam->getRealPosition();
}

Quaternion PilotControls::cameraOrientation() {
    return cam->getRealOrientation();
}

void PilotControls::setEnabled(bool b)
{
    enabled = b;
}

bool PilotControls::isEnabled() { return enabled; }

void PilotControls::tick()
{
    if(enabled) {
        if(inputState->isKeyDown(OIS::KC_W))
            mForward = 1.0;
       	else if(inputState->isKeyDown(OIS::KC_S))
            mForward = -1.0;
       	else
       		mForward = 0.0;
        
        if(inputState->isKeyDown(OIS::KC_D))
        	mSide = 1.0;
       	else if(inputState->isKeyDown(OIS::KC_A))
       		mSide = -1.0;
       	else
       		mSide = 0.0;
        
        if(inputState->isKeyDown(OIS::KC_SPACE))
            mUp = 1.0;
       	else if(inputState->isKeyDown(OIS::KC_LSHIFT) || inputState->isKeyDown(OIS::KC_LCONTROL))
       		mUp = -1.0;
       	else
       		mUp = 0.0;
      
      	if(inputState->isKeyDown(OIS::KC_UP))
        	mPitch = 1.0;
       	else if(inputState->isKeyDown(OIS::KC_DOWN))
       		mPitch = -1.0;
       	else
       		mPitch = 0.0;
            
        if(inputState->isKeyDown(OIS::KC_LEFT))
        	mYaw = 1.0;
       	else if(inputState->isKeyDown(OIS::KC_RIGHT))
       		mYaw = -1.0;
       	else
       		mYaw = 0.0;
       		
        isFire = inputState->isKeyDown(OIS::KC_RCONTROL);
    }
}

PilotControls::PilotControls(InputState *inputState, Camera *cam)
    : inputState(inputState)
    , cam(cam)
    , enabled(true)
{}

PilotControls::~PilotControls() {}

