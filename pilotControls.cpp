
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

void PilotControls::tick()
{
    tickGunnerControls();

    if(isEnabled()) {
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
        
        if(inputState->isKeyDown(OIS::KC_SPACE)
        || inputState->isKeyDown(OIS::KC_UP))
            mUp = 1.0;
        else if(inputState->isKeyDown(OIS::KC_LSHIFT)
             || inputState->isKeyDown(OIS::KC_LCONTROL)
             || inputState->isKeyDown(OIS::KC_DOWN))
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
    }
}

PilotControls::PilotControls(InputState *inputState, Camera *cam)
    : inputState(inputState)
    , GunnerControls(inputState, cam, false)
{
    // Lock pilots camera
    setYawLimits(0,0);
    setPitchLimits(0,0);
}

PilotControls::~PilotControls() {}

