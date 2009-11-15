#ifndef ACCELERATION_STATE_H
#define ACCELERATION_STATE_H

// Includes all Ogre classes
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
    virtual double propForwardVel(){ return vPropForwardVel; }
	virtual double propSideVel(){ return vPropSideVel; }
    virtual double propUpVel(){ return vPropUpVel; }
	virtual double propYawVel(){ return vPropYawVel; }
	virtual double propPitchVel(){ return vPropPitchVel; }
    virtual void tick();
    AccelerationState( IInput *fromInput );
}

#endif
