
#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include <OGRE/Ogre.h>

#include "IMotionState.h"
#include "IAccelerationState.h"
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
    
    IAccelerationState *engineState;
    
    //constant values that will need tuning
    static const double FORWARD_SPEED= 2;
    static const double SIDE_SPEED = 0.5;
    static const double TURN_SPEED = 0.0175;
    static const double UP_SPEED = 0.5;
    
    static const double MAX_BANK = 0.2;
    
  public:
    MotionState(IAccelerationState *as);
    double xVelocity();
    double yVelocity();
    double zVelocity();
    
    double pitch();
    double yaw();
    double roll();
    void tick();
};

#endif
