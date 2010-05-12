#include "splashScreen.h"

SplashScreen::SplashScreen(InputState *inputState,
                           GuiManager *guiMgr)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , isVisible(false)
    , isEnd(false)
{
    CEGUI::ImagesetManager::getSingleton().create("title.xml");
}

void SplashScreen::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_SPACE)) {
         isEnd = true;
    } else if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(0);
    }
}

MenuType SplashScreen::nextMenu() {
    return MT_CHOOSE_NICK;
}

void SplashScreen::show() {
    // Show background image etc
    if (isVisible) return;

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float wpixel = winWidth / 1680.0;
    float hpixel = winHeight / 1050.0;

    guiMgr->addStaticImage("Title",0.5, 0.5,1.0, 1.0,"Title","Title");
    instructions = guiMgr->addStaticText("", "Press SPACE to continue", 0.5, 0.4, 1);

    isVisible = true;
}

void SplashScreen::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Title");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(instructions);

}

bool SplashScreen::end() { return isEnd; }

bool SplashScreen::visible() { return isVisible; }
