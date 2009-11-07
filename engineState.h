#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

#include "IEngineState.h"

class EngineState : public IEngineState {
private:

    double mEnginePower;
    OIS::Keyboard* mKeyboard;
   	OIS::InputManager* mInputManager;
  	void setEnginePower(char);

public:

	virtual double enginePower();
	virtual double sideThrusterPower();
	virtual double turnPower();
	virtual double pitchPower();
	
	virtual void tick();

    EngineState(bool bufferedKeys = false) :
        mKeyboard(0)
    {
        using namespace OIS;
        ParamList pl;
        mInputManager = InputManager::createInputSystem(pl);
        
        mKeyboard = static_cast<Keyboard*> (mInputManager->createInputObject(OISKeyboard, bufferedKeys));
        }

};

#endif
