#include "navigatorControls.h"
#include "const.h"

NavigatorControls::NavigatorControls(InputState *inputState, Camera *cam) 
    : inputState(inputState)
    , GunnerControls(inputState, cam)
    , isFullMap(false)
{
    setYawLimits(0, 2 * PI);
    setPitchLimits(7 * PI / 4,PI / 2);
    setLook(0, 0);
}

NavigatorControls::~NavigatorControls()
{}

bool NavigatorControls::isMap() {
    return isFullMap; 
}

void NavigatorControls::tick()
{
    tickGunnerControls();

    if(isEnabled()) {
        isFullMap = inputState->isKeyDown(OIS::KC_TAB);
    }
}
