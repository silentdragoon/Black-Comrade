
#ifndef MOTION_STATE_H
#define MOTION_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "IMotionState.h"
#include "IEngineState.h"

using namespace Ogre;

class MotionState : public IMotionState
{
  private:
    double xMotion;
    double yMotion;
    double zMotion;
    double pitchAngleS;
    double rollAngleS;
    double yawAngleS;
    
    IEngineState *engineState;
    
    //double xForce;
    //double yForce;
    
    //constant values that will need tuning
    
    static const double MASS = 100.0;
    static const double ENGINEPOWER = 10;
    static const double SIDETHURSTERPOWER = 10;
    static const double xFRIC = 0.95;
    static const double yFRIC = 0.8;
    static const double PI = 3.1415926535897932;
    
  public:
    MotionState(IEngineState *es);
    double xVelocity();
    double yVelocity();
    double zVelocity();
    
    double pitchVelocity();
    double yawVelocity();
    double rollVelocity();
    void tick();
};

#endif
