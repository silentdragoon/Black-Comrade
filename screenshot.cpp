#include "screenshot.h"

screenshot::screenshot(InputState *inputState, Ogre::RenderWindow *window) :
    inputState(inputState),
    window(window)
    {}

screenshot::~screenshot() {}

void screenshot::tick() {

    if (inputState->isKeyDown(OIS::KC_SYSRQ)) {
        

}
