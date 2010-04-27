#include "engineerControls.h"
#include "const.h"

EngineerControls::EngineerControls(InputState *inputState, Camera *cam) :
    inputState(inputState),
    isChangeEngine(false),
    isChangeWeapons(false),
    isTransferShields(false),
    isTransferWeapons(false)
    
    , GunnerControls(inputState, cam)
{
    setYawLimits(0, 2 * PI);
    setPitchLimits(3 * PI / 2,PI / 4);
}

EngineerControls::~EngineerControls()
{}

bool EngineerControls::isEngine() {
    return isChangeEngine;
}

bool EngineerControls::isWeapons() {
    return isChangeWeapons;
}

bool EngineerControls::transferShields() {
    return isTransferShields;
}

bool EngineerControls::transferWeapons() {
    return isTransferWeapons;
}

void EngineerControls::tick()
{
    tickGunnerControls();

    if(isEnabled()) {
        isChangeEngine = inputState->isKeyDown(OIS::KC_D);
        isChangeWeapons = inputState->isKeyDown(OIS::KC_A);
        isTransferShields = inputState->isKeyDown(OIS::KC_W);
        isTransferWeapons = inputState->isKeyDown(OIS::KC_Q);
    }
}
