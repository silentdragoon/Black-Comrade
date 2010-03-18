#include "navigatorControls.h"
#include "const.h"

NavigatorControls::NavigatorControls(InputState *inputState, Camera *cam) 
    : inputState(inputState)
    , GunnerControls(inputState, cam)
    , isFullMap(false)
{
    setYawLimits(PI, 2 * PI);
    setPitchLimits(3 * PI / 4, PI / 4);
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
