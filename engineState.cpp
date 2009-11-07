#include "engineState.h"

double EngineState::enginePower()
{   return 1.0;
    // keep Engine Power between 0.0 and 1.0
    if (mEnginePower >= 1.0)
        mEnginePower = 1.0;
    if (mEnginePower <= 0.0)
        mEnginePower = 0.0;
    return mEnginePower;
}

double EngineState::sideThrusterPower()
{
    return 0.0;
}

double EngineState::turnPower()
{
    return 0.0;
}

double EngineState::pitchPower()
{
    return 0.0;
}

void EngineState::setEnginePower(char input)
{
    switch (input) {
        case 'w':
            mEnginePower += 0.1;
            break;
        case 's':
            mEnginePower -= 0.1;
            break;
    }
}

void EngineState::tick()
{
    mKeyboard->capture();
    if(mKeyboard->isKeyDown(OIS::KC_W))
        setEnginePower('w');
    if(mKeyboard->isKeyDown(OIS::KC_S))
        setEnginePower('s');
    /*
    if(mKeyboard->isKeyDown(OIS::KC_A))

    if(mKeyboard->isKeyDown(OIS::KC_D))

    if(mKeyboard->isKeyDown(OIS::KC_UP))	

    if(mKeyboard->isKeyDown(OIS::KC_DOWN))

    if(mKeyboard->isKeyDown(OIS::KC_LEFT))

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT))*/
}

EngineState::EngineState(RenderWindow *window, bool bufferedKeys = false) :
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
