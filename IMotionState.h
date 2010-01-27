
#ifndef I_MOTION_STATE_H
#define I_MOTION_STATE_H

class IMotionState {
public:
	virtual ~IMotionState() {}
	
	virtual double xVelocity() = 0;
	virtual double yVelocity() = 0;
	virtual double zVelocity() = 0;
	
	virtual double pitch() = 0;
	virtual double yaw() = 0;
	virtual double roll() = 0;
};

#endif
