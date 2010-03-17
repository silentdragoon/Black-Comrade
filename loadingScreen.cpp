#include "loadingScreen.h"
#include "IMenuScreen.h"

LoadingScreen::LoadingScreen(InputState *inputState, GuiManager *guiMgr)
    : inputState(inputState)
    , guiMgr(guiMgr)
{
    CEGUI::ImagesetManager::getSingleton().create("loading.xml");
}

void LoadingScreen::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(-1);
    }
}

MenuType::LoadingScreen::nextMenu() {
    return MT_NONE;
}

void LoadingScreen::show() {
    // Show background image etc
    if (isVisible) return;

    //CEGUI::FrameWindow *loadingText = guiMgr->addStaticText("LoadingText", "Loading...",0.5, 0.5, 1);
    CEGUI::FrameWindow *loadingBackground = guiMgr->addStaticImage("Loading",0.5, 0.5,1.0, 1.0,"Loading","Loading");
    isVisible = true;
}

void LoadingScreen::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Loading");
}

bool LoadingScreen::end() { return false; }

bool LoadingScreen::visible() { return isVisible; }
