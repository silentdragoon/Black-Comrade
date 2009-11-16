#include "engineState.h"
using namespace std;

double EngineState::enginePower()
{   
    // keep Engine Power between -1.0 and 1.0
    /*
    if (mEnginePower >= 1.00)
        mEnginePower = 1.00;
    if (mEnginePower <= -1.00)
        mEnginePower = -1.00;
    */
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

double EngineState::upThrusterPower()
{
    if (mUpThrusterPower >= 1.00)
        mUpThrusterPower = 1.00;
    if (mUpThrusterPower <= -1.00)
        mUpThrusterPower = -1.00;
    return mUpThrusterPower;
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
    
    //cout<< "mEnPow: " << mEnginePower;
    if(mKeyboard->isKeyDown(OIS::KC_W))
    {
        if( mEnginePower < 0 ) mEnginePower += 0.5 *ENGINEPOWER / MASS;
        else mEnginePower += ENGINEPOWER / (MASS *4*(mEnginePower+1));
        
    }
   	else if(mKeyboard->isKeyDown(OIS::KC_S))
    {
        if( mEnginePower > 0 ) mEnginePower -= 0.5 * ENGINEPOWER / MASS;
   		else mEnginePower -= ENGINEPOWER / (MASS *4*(-mEnginePower+1));
    }
   	else
   		mEnginePower *= zFRIC;
    
    if(mKeyboard->isKeyDown(OIS::KC_D))
    	mSideThrusterPower += SIDETHURSTERPOWER / MASS;
   	else if(mKeyboard->isKeyDown(OIS::KC_A))
   		mSideThrusterPower -= SIDETHURSTERPOWER / MASS;
   	else
   		mSideThrusterPower *= xFRIC;
    
    if(mKeyboard->isKeyDown(OIS::KC_LSHIFT) || mKeyboard->isKeyDown(OIS::KC_LCONTROL))
    {
        cout<< "UP"<< endl;
    	mUpThrusterPower += SIDETHURSTERPOWER / MASS;
    }
   	else if(mKeyboard->isKeyDown(OIS::KC_SPACE))
   		mUpThrusterPower -= SIDETHURSTERPOWER / MASS;
   	else
   		mUpThrusterPower *= xFRIC;
  
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

EngineState::EngineState(RenderWindow *window, bool bufferedKeys) 
    : mKeyboard(0)
    , mEnginePower(0)
    , mSideThrusterPower(0)
    , mUpThrusterPower(0)
    , mPitchPower(0)
    , mTurnPower(0)
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
