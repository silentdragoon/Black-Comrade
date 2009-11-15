#ifndef ACCELERATION_STATE_H
#define ACCELERATION_STATE_H

#include <OGRE/Ogre.h>

#include "IInput.h"
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
    IInput *input;
    
  public:
    virtual double propForwardVel();
	virtual double propSideVel();
    virtual double propUpVel();
	virtual double propYawVel();
	virtual double propPitchVel();
    virtual void tick();
    AccelerationState( IInput *fromInput );
}

#endif
