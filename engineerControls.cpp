#include "engineerControls.h"
#include "const.h"

EngineerControls::EngineerControls(InputState *inputState, Camera *cam) :
    inputState(inputState),
    isChangeShield(false),
    isChangeWeapons(false),
    isChangeSensors(false),
    isTransferShields(false),
    isTransferWeapons(false)
    
    , GunnerControls(inputState, cam)
{
    setYawLimits(0, PI);
    setPitchLimits(3 * PI / 4, PI / 4);
}

EngineerControls::~EngineerControls()
{}

bool EngineerControls::isShield() {
    return isChangeShield;
}

bool EngineerControls::isWeapons() {
    return isChangeWeapons;
}

bool EngineerControls::isSensors() {
    return isChangeSensors;
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
        isChangeShield = inputState->isKeyDown(OIS::KC_S);
        isChangeWeapons = inputState->isKeyDown(OIS::KC_A);
        isChangeSensors = inputState->isKeyDown(OIS::KC_D);
        isTransferShields = inputState->isKeyDown(OIS::KC_W);
        isTransferWeapons = inputState->isKeyDown(OIS::KC_Q);
    }
}
