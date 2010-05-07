#include "screenshot.h"

Screenshot::Screenshot(InputState *inputState, Ogre::RenderWindow *window)
	: inputState(inputState)
    , window(window)
{}

Screenshot::~Screenshot() {}

std::string Screenshot::createUnique(std::string name) {
    std::stringstream out;
    out << num++;
    name+=out.str();
    return name;
}

void Screenshot::tick() {

    if (inputState->isKeyDown(OIS::KC_SYSRQ)) {
		window->writeContentsToFile(createUnique("screenshot") + ".png");
    }

}

