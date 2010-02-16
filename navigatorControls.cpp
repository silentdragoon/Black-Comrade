#include "navigatorControls.h"
#include "const.h"

NavigatorControls::NavigatorControls(InputState *inputState, Camera *cam) :
    inputState(inputState),
    cam(cam),
    enabled(true),
    isFire(false),
    isFullMap(false)
{}

NavigatorControls::~NavigatorControls()
{}

bool NavigatorControls::fire() {
    return isFire;
}

bool NavigatorControls::isMap() {
    return isFullMap; 
}

Vector3 NavigatorControls::cameraPosition() {
    return cam->getRealPosition();
}

Quaternion NavigatorControls::cameraOrientation() {
    return cam->getRealOrientation();
}

void NavigatorControls::tick()
{
    if(enabled) {
        int x = inputState->getMouseX();
        int y = inputState->getMouseY();

        cam->yaw(Degree(Const::TURRET_SPEED * x));
        cam->pitch(Degree(Const::TURRET_SPEED * y));

        isFire = inputState->isMouseButtonDown(OIS::MB_Left);
        isFullMap = inputState->isKeyDown(OIS::KC_TAB);
    }
}
