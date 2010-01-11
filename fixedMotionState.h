
#ifndef FIXED_MOTION_STATE_H
#define FIXED_MOTION_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "IMotionState.h"

using namespace Ogre;

class FixedMotionState : public IMotionState {
private:

public:
    
    virtual double xVelocity() { return 0.0; }
	virtual double yVelocity() { return 0.0; }
	virtual double zVelocity() { return 0.06; }
	
	virtual double pitchVelocity() { return 0.0; }
	virtual double yawVelocity() { return 0.0; }
	virtual double rollVelocity() { return 0.005; }
	
	virtual void tick() {}
};

#endif
