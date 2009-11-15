#include "motionState.h"
#include <iostream>
#include <math.h>

using namespace std;

MotionState::MotionState(IAccelerationState *as):
    engineState(as),
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
    
    std::cout << engineState->propForwardVel() << std::endl;
    
    mYaw += engineState->propYawVel() * 0.01;
    
    mRoll = engineState->propYawVel() * abs(engineState->propForwardVel() / 1.3) * 0.07;
    
    zMotion = cos(mYaw) * engineState->propForwardVel() + sin(mYaw) * (engineState->propSideVel());
    xMotion = cos(mYaw) * (-engineState->propSideVel()) + sin(mYaw) * engineState->propForwardVel();
    yMotion = engineState->propUpVel();

    //double xPowerFrac = engineState->sideThrusterPower();
    //yMotion = (xMotion + xPowerFrac*SIDETHURSTERPOWER / MASS) * yFRIC;
    //cout << "xMotion: " << xMotion << endl;
    
    //mYaw = engineState->turnPower() * PI * TURNPERTICK;
    //cout << "yawAngleS: " << yawAngleS << endl;
    //pitchAngleS = engineState->pitchPower() * PI * TURNPERTICK ;
    //cout << "pitchAngleS: " << pitchAngleS << endl;
}
