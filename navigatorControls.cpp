#include "navigatorControls.h"
<<<<<<< HEAD:navigatorControls.cpp
#include "const.h"
=======
#include <iostream>
>>>>>>> 84224194c65ec8f50f96274c654977febdd37848:navigatorControls.cpp

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
<<<<<<< HEAD:navigatorControls.cpp
        cam->yaw(Degree(Const::TURRET_SPEED * x));
        cam->pitch(Degree(Const::TURRET_SPEED * y));
=======

        cam->yaw(Degree(-0.13 * x));
        cam->pitch(Degree(-0.13 * y));
>>>>>>> 84224194c65ec8f50f96274c654977febdd37848:navigatorControls.cpp
    }
}
