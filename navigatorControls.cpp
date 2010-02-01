#include "navigatorControls.h"

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
        cam->yaw(Degree(-0.13 * x));
        cam->pitch(Degree(-0.13 * y));
    }
}
