#include "engineState.h"

double EngineState::enginePower()
{   
    // keep Engine Power between -1.0 and 1.0
    if (mEnginePower >= 1.00)
        mEnginePower = 1.00;
    if (mEnginePower <= -1.00)
        mEnginePower = -1.00;
    return mEnginePower;
}

double EngineState::sideThrusterPower()
{
    if (mSideThrusterPower >= 1.00)
        mSideThrusterPower = 1.00;
    if (mSideThrusterPower <= -1.00)
        mSideThrusterPower = -1.00;
    return mSideThrusterPower;
}

double EngineState::turnPower()
{
    if (mTurnPower >= 1.00)
        mTurnPower = 1.00;
    if (mTurnPower <= -1.00)
        mTurnPower = -1.00;
    return mTurnPower;
}

double EngineState::pitchPower()
{
    if (mPitchPower >= 1.00)
        mPitchPower = 1.00;
    if (mPitchPower <= -1.00)
        mPitchPower = -1.00;
    return mPitchPower;
}

void EngineState::tick()
{
    mKeyboard->capture();
    
    
    if(mKeyboard->isKeyDown(OIS::KC_W))
    	mEnginePower += ENGINEPOWER / MASS;
   	else if(mKeyboard->isKeyDown(OIS::KC_S))
   		mEnginePower -= ENGINEPOWER / MASS;
   	else
   		mEnginePower *= xFRIC;
    
    if(mKeyboard->isKeyDown(OIS::KC_W))
    	mSideThrusterPower = 1;
   	else if(mKeyboard->isKeyDown(OIS::KC_S))
   		mSideThrusterPower = -1;
   	else
   		mSideThrusterPower = 0;
  
  	if(mKeyboard->isKeyDown(OIS::KC_UP))
    	mPitchPower = 1;
   	else if(mKeyboard->isKeyDown(OIS::KC_DOWN))
   		mPitchPower = -1;
   	else
   		mPitchPower = 0;
        
    if(mKeyboard->isKeyDown(OIS::KC_LEFT))
    	mTurnPower = 1;
   	else if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
   		mTurnPower = -1;
   	else
   		mTurnPower = 0;    
    
           
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        exit(0);    
}

EngineState::EngineState(RenderWindow *window, bool bufferedKeys) :
        mKeyboard(0)
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);
    mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeys));

        
        
}
