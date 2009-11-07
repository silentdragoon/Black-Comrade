#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "IEngineState.h"

class EngineState : public IEngineState {
private:

public:

	virtual double enginePower();
	virtual double sideThrusterPower();
	virtual double turnPower();
	virtual double pitchPower();
	
	virtual void tick();
};

#endif
