
#ifndef I_ENGINE_STATE_H
#define I_ENGINE_STATE_H

class IEngineState {
public:
	virtual ~IEngineState() {}
	
	// Values should be in the range [0,1]
	virtual double enginePower() = 0;
	virtual double sideThrusterPower() = 0;
	virtual double turnPower() = 0;
	virtual double pitchPower() = 0;
};

#endif
