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

    OIS::Keyboard* mKeyboard;
    OIS::InputManager* mInputManager;
    
    static const double MASS = 10000.0;
    static const double ENGINEPOWER = 25;
    static const double SIDETHURSTERPOWER = 25;
    static const double xFRIC = 0.996;
    static const double yFRIC = 0.996;
    static const double PI = 3.1415926535897932;
    
public:

	virtual double enginePower();
	virtual double sideThrusterPower();
	virtual double turnPower();
	virtual double pitchPower();
	
	virtual void tick();
	EngineState(RenderWindow *window, bool bufferedKeys = false);

};

#endif
