
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
