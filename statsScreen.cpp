#include "statsScreen.h"
#include "IMenuScreen.h"

StatsScreen::StatsScreen(InputState *inputState, GuiManager *guiMgr)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , isEnd(false)
    , isVisible(false)
{
    //CEGUI::ImagesetManager::getSingleton().create("LoadingScreen.imageset");
}

void StatsScreen::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_SPACE)) {
        isEnd = true;
        std::cout << "got space" << std::endl;
    }
    std::cout << "ticking stats" << std::endl;
}

MenuType::StatsScreen::nextMenu() {
    return MT_NONE;
}

void StatsScreen::show() {
    // Show background image etc
    if (isVisible) return;

    CEGUI::FrameWindow *loadingText = guiMgr->addStaticText("StatsText", "Stats here...(space bar to exit)",0.5, 0.5, 1);
    isVisible = true;
}

void StatsScreen::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("StatsText");
}

bool StatsScreen::end() { return isEnd; }

bool StatsScreen::visible() { return isVisible; }
