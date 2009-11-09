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

void EngineState::setEnginePower(char input)
{
    switch (input) {
        case 'w':
            mEnginePower += 0.05;
            break;
        case 's':
            mEnginePower -= 0.05;
            break;
    }
}

void EngineState::setSideThrusterPower(char input)
{
    switch (input) {
        case 'a':
            mSideThrusterPower += 0.05;
            break;
        case 'd':
            mSideThrusterPower -= 0.05;
            break;
    }
}

void EngineState::setTurnPower(char input)
{
    switch (input) {
        case 'r':
            mTurnPower += 0.001;
            break;
        case 'l':
            mTurnPower -= 0.001;
            break;
    }
}

void EngineState::setPitchPower(char input)
{
    switch (input) {
        case 'd':
            mPitchPower += 0.001;
            break;
        case 'u':
            mPitchPower -= 0.001;
            break;
        case 'r':
            mPitchPower = 0;
            break;
    }
}

void EngineState::tick()
{
    mKeyboard->capture();
    if (mKeyboard->isKeyDown(OIS::KC_W))
        setEnginePower('w');
    if (mKeyboard->isKeyDown(OIS::KC_S))
        setEnginePower('s');
    if (mKeyboard->isKeyDown(OIS::KC_A))
        setSideThrusterPower('a');
    if (mKeyboard->isKeyDown(OIS::KC_D))
        setSideThrusterPower('d');
        
    if (mKeyboard->isKeyDown(OIS::KC_UP))	
        setPitchPower('u');
    else
        setPitchPower('r');
    if (mKeyboard->isKeyDown(OIS::KC_DOWN))
        setPitchPower('d');
    else
        setPitchPower('r');
        
    if (mKeyboard->isKeyDown(OIS::KC_LEFT))
        setTurnPower('l');
    if (mKeyboard->isKeyDown(OIS::KC_RIGHT))
       setTurnPower('r');
       
       
       
       
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
