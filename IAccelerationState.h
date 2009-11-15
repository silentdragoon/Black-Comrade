
#ifndef I_ACCELERATION_STATE_H
#define I_ACCELERATION_STATE_H

class IAccelerationState {
public:
	virtual ~IAccelerationState() {}
	
	// Values should be in the range [-1,1]
    //naming convention comes from proportional _____ Velocity
	virtual double propForwardVel() = 0;
	virtual double propSideVel() = 0;
    virtual double propUpVel() = 0;
	virtual double propYawVel() = 0;
	virtual double propPitchVel() = 0;
};

#endif
