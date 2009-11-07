#include "motionState.h"

MotionState::MotionState(IEngineState *es):
    engineState(es),
    xMotion(0.0),
    yMotion(0.0),
    zMotion(0.0),
    pitchAngleS(0.0),
    rollAngleS(0.0),
    yawAngleS(0.0)
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

double MotionState::pitchVelocity()
{
    return pitchAngleS;
}

double MotionState::yawVelocity()
{
    return rollAngleS;
}

double MotionState::rollVelocity()
{
    return yawAngleS;
}

void MotionState::tick()
{
    double xPowerFrac = engineState->enginePower();
    xMotion = (xMotion + (xPowerFrac*ENGINEPOWER) / MASS) * xFRIC;
    
    double yPowerFrac = engineState->sideThrusterPower();
    yMotion = (yMotion + yPowerFrac*SIDETHURSTERPOWER / MASS) * yFRIC;
    
    yawAngleS = engineState->turnPower() * 2 * PI;
    pitchAngleS = engineState->pitchPower() * 2 * PI;
}
