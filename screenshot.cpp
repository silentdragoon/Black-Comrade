#include "screenshot.h"

Screenshot::Screenshot(InputState *inputState, Ogre::RenderWindow *window)
	: inputState(inputState)
    , window(window)
{}

Screenshot::~Screenshot() {}

void Screenshot::tick() {

    if (inputState->isKeyDown(OIS::KC_SYSRQ)) {
		window->writeContentsToFile("screenshot.png");
    }

}

