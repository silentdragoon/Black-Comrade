#ifndef ACCELERATION_STATE_H
#define ACCELERATION_STATE_H

#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "IAccelerationState.h"

class AccelerationState : public IAccelerationState, public ITickable
{
  private:
    double vPropForwardVel;
    double vPropSideVel;
    double vPropUpVel;
    double vPropYawVel;
    double vPropPitchVel;
    IAccelerationState *input;
    
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
