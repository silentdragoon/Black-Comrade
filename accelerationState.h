#ifndef ACCELERATION_STATE_H
#define ACCELERATION_STATE_H

#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "IAccelerationState.h"
#include <math.h>
#include <iostream>

using namespace std;

class AccelerationState : public IAccelerationState, public ITickable
{
  private:
    double vPropForwardVel;
    double cforward;
    double vPropSideVel;
    double vPropUpVel;
    double vPropYawVel;
    double vPropPitchVel;
    IAccelerationState *input;
    
    double cForward;
    double cSide;
    double cUp;
    double cYaw;
    double cPitch;
    
    //ticks required to reach ~95%
    static const double TICKSFORWARD = 200;
    static const double TICKSSIDE = 100;
    static const double TICKSUP = 50;
    static const double TICKSYAW = 20;
    static const double TICKSPITCH = 20;
    
  public:
    virtual double forward();
	virtual double side();
    virtual double up();
	virtual double yaw();
	virtual double pitch();
    virtual void tick();
    AccelerationState( IAccelerationState *fromInput );
};

#endif
