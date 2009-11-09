#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include "IEngineState.h"
#include "ITickable.h"
#include <OGRE/Ogre.h>
#include <iostream>

using namespace Ogre;

class EngineState : public IEngineState, public ITickable {

private:

    double mEnginePower;
    double mSideThrusterPower;
    double mPitchPower;
    double mTurnPower;
    
    void setEnginePower(char);
    void setSideThrusterPower(char);
    void setPitchPower(char);
    void setTurnPower(char);
    
    OIS::Keyboard* mKeyboard;
    OIS::InputManager* mInputManager;
    
public:

	virtual double enginePower();
	virtual double sideThrusterPower();
	virtual double turnPower();
	virtual double pitchPower();
	
	virtual void tick();
	EngineState(RenderWindow *window, bool bufferedKeys = false);

};

#endif
