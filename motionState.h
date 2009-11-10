
#ifndef MOTION_STATE_H
#define MOTION_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "IMotionState.h"
#include "IEngineState.h"
#include "ITickable.h"

using namespace Ogre;

class MotionState : public IMotionState, public ITickable
{
  private:
    double xMotion;
    double yMotion;
    double zMotion;
    double mPitch;
    double mRoll;
    double mYaw;
    
    IEngineState *engineState;
    
    //double xForce;
    //double yForce;
    
    //constant values that will need tuning
    
    static const double MASS = 10000.0;
    static const double ENGINEPOWER = 25;
    static const double SIDETHURSTERPOWER = 25;
    static const double xFRIC = 0.996;
    static const double yFRIC = 0.996;
    static const double PI = 3.1415926535897932;
    // in deg/180 = TURNPERTICK at max value turn value
    static const double TURNPERTICK = 0.01;
    
  public:
    MotionState(IEngineState *es);
    double xVelocity();
    double yVelocity();
    double zVelocity();
    
    double pitch();
    double yaw();
    double roll();
    void tick();
};

#endif
