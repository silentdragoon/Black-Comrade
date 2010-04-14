#include "loadingScreen.h"
#include "IMenuScreen.h"

LoadingScreen::LoadingScreen(InputState *inputState, GuiManager *guiMgr,
                             NetworkingManager *networkingMgr)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , networkingMgr(networkingMgr)
    , progress(0)
    , isEnd(false)
{
    CEGUI::ImagesetManager::getSingleton().create("loading.xml");
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();

    float g = (1.0*winWidth)/1680.0;
    wpixel = 1.0 / (float)winWidth * g;
    hpixel = 1.0 / (float)winHeight * g;
    isVisible = false;
}

void LoadingScreen::tick() {
    // Check for key presses etc
    if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(-1);
    }
    networkingMgr->tick();
    if (progress >= 100 ) isEnd = true;
}

MenuType::LoadingScreen::nextMenu() {
    return MT_NONE;
}

void LoadingScreen::show() {
    // Show background image etc

    if (isVisible) return;
   
    CEGUI::MouseCursor::getSingletonPtr()->hide();

    switch (networkingMgr->collabInfo->getGameRole()) {
        case PILOT:
            loadingBackground = guiMgr->addStaticImage("KeyboardPilot",0.5, 0.5,1.0, 1.0,"KeyboardPilot","Loading");
            break;
        case NAVIGATOR:
            loadingBackground = guiMgr->addStaticImage("KeyboardNavigator",0.5, 0.5,1.0, 1.0,"KeyboardNavigator","Loading");
            break;
        case ENGINEER:
            loadingBackground = guiMgr->addStaticImage("KeyboardEngineer",0.5, 0.5,1.0, 1.0,"KeyboardEngineer","Loading");
            break;
    }

    indicator = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","loadingIndicator"));
    instructions = guiMgr->addStaticText("", "When you are familiar with your controls, press the space bar to begin the mission", 0.5, 0.89, 1);
    instructions->hide();

    guiMgr->getRootWindow()->addChildWindow(indicator);
    indicator->setSize(CEGUI::UVector2(CEGUI::UDim(210 * wpixel,0),CEGUI::UDim(210 * hpixel,0)));
    indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(1 - 30 * hpixel,0),CEGUI::UDim(1 - 210 * wpixel,0)));
    indicator->setText("0%");

    isVisible = true;
}

void LoadingScreen::render() {
    WindowEventUtilities weu = WindowEventUtilities();
    weu.messagePump();
    Root::getSingletonPtr()->renderOneFrame();
}

int LoadingScreen::getProgress() { return progress; }

void LoadingScreen::updateProgress(int newProgress) {
    progress = newProgress;    std::stringstream out;
    
    if (progress != 100.0) {
        out << progress << "%";
        indicator->setText(out.str());
    } else {
        // Let the player know the game has loaded fully
        indicator->setText("");
        instructions->show();
    }
    render();
    inputState->tick();
    if (inputState->isKeyDown(OIS::KC_ESCAPE)) std::exit(0);
}

void LoadingScreen::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(loadingBackground);
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(indicator);
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(instructions);
}

bool LoadingScreen::end() {
    return isEnd;
}

bool LoadingScreen::visible() { return isVisible; }
