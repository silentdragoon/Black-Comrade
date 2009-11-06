
#ifndef I_MOTION_STATE_H
#define I_MOTION_STATE_H

class IMotionState {
public:
	virtual ~IMotionState() {}
	
	virtual double xVelocity() = 0;
	virtual double yVelocity() = 0;
	virtual double zVelocity() = 0;
	
	virtual double pitchVelocity() = 0;
	virtual double yawVelocity() = 0;
	virtual double rollVelocity() = 0;
	
};

#endif
