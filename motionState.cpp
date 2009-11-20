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
    mYaw += engineState->yaw() * TURN_SPEED;
    

    mRoll = -engineState->yaw() * abs(engineState->forward()) * MAX_BANK;

    double forwardV = engineState->forward() * FORWARD_SPEED;
    double sideV = engineState->side() * SIDE_SPEED;
    double upV = engineState->up() * UP_SPEED;

    
    zMotion = cos(mYaw) * forwardV + sin(mYaw) * (sideV);
    xMotion = cos(mYaw) * (-sideV) + sin(mYaw) * forwardV;
    yMotion = upV;

    std::cout << forwardV << std::endl;    
}
