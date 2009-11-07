#include "motionState.h"
#include <iostream>
using namespace std;

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
    
    double zPowerFrac = engineState->enginePower();
    zMotion = (zMotion + (zPowerFrac*ENGINEPOWER) / MASS) * xFRIC;
    cout << "zMotion: " << zMotion << endl;
    
    double xPowerFrac = engineState->sideThrusterPower();
    yMotion = (xMotion + xPowerFrac*SIDETHURSTERPOWER / MASS) * yFRIC;
    cout << "xMotion: " << xMotion << endl;
    
    yawAngleS = engineState->turnPower() * 2 * PI;
    cout << "yawAngleS: " << yawAngleS << endl;
    pitchAngleS = engineState->pitchPower() * 2 * PI;
    cout << "pitchAngleS: " << pitchAngleS << endl;
}
