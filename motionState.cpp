#include "motionState.h"
#include <iostream>
using namespace std;

MotionState::MotionState(IEngineState *es):
    engineState(es),
    xMotion(0.0),
    yMotion(0.0),
    zMotion(0.0),
    mPitch(0.0),
    mRoll(0.0),
    mYaw(0.0)
{}

double MotionState::xVelocity()
{
    return xMotion;
}

double MotionState::yVelocity()
{
    return yMotion;
}

double MotionState::zVelocity()
{
    return zMotion;
}

double MotionState::pitch()
{
    return mPitch;
}

double MotionState::yaw()
{
    return mYaw;
}

double MotionState::roll()
{
    return mRoll;
}

void MotionState::tick()
{
    
    zMotion = engineState->enginePower();
    
    //double xPowerFrac = engineState->sideThrusterPower();
    //yMotion = (xMotion + xPowerFrac*SIDETHURSTERPOWER / MASS) * yFRIC;
    //cout << "xMotion: " << xMotion << endl;
    
    //mYaw = engineState->turnPower() * PI * TURNPERTICK;
    //cout << "yawAngleS: " << yawAngleS << endl;
    //pitchAngleS = engineState->pitchPower() * PI * TURNPERTICK ;
    //cout << "pitchAngleS: " << pitchAngleS << endl;
}
