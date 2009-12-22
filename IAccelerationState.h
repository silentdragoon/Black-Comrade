
#ifndef I_ACCELERATION_STATE_H
#define I_ACCELERATION_STATE_H

class IAccelerationState {
public:
	virtual ~IAccelerationState() {}
	
	// Values should be in the range [-1,1]
    //naming convention comes from proportional _____ Velocity
	virtual double forward() = 0;
	virtual double side() = 0;
    virtual double up() = 0;
	virtual double yaw() = 0;
	virtual double pitch() = 0;
};

#endif
