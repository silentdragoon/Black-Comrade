#include "navigatorControls.h"
#include "const.h"

NavigatorControls::NavigatorControls(InputState *inputState, Camera *cam) :
    inputState(inputState),
    cam(cam),
    enabled(true)
{}

NavigatorControls::~NavigatorControls()
{}

void NavigatorControls::tick()
{
    if(enabled) {
        int x = inputState->getMouseX();
        int y = inputState->getMouseY();

        cam->yaw(Degree(Const::TURRET_SPEED * x));
        cam->pitch(Degree(Const::TURRET_SPEED * y));
    }
}
