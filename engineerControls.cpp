#include "engineerControls.h"
#include "const.h"

EngineerControls::EngineerControls(InputState *inputState, Camera *cam) :
    inputState(inputState),
    cam(cam),
    enabled(true),
    isFire(false)
{}

EngineerControls::~EngineerControls()
{}

bool EngineerControls::fire() {
    return isFire;
}

Vector3 EngineerControls::cameraPosition() {
    return cam->getRealPosition();
}

Quaternion EngineerControls::cameraOrientation() {
    return cam->getRealOrientation();
}

void EngineerControls::tick()
{
    if(enabled) {
        int x = inputState->getMouseX();
        int y = inputState->getMouseY();
        cam->yaw(Degree(Const::TURRET_SPEED * x));
        cam->pitch(Degree(Const::TURRET_SPEED * y));

        isFire = inputState->isMouseButtonDown(OIS::MB_Left);
    }
}
