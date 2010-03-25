#include "loadingScreen.h"
#include "IMenuScreen.h"

LoadingScreen::LoadingScreen(InputState *inputState, GuiManager *guiMgr)
    : inputState(inputState)
    , guiMgr(guiMgr)
    , progress(0)
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
}

MenuType::LoadingScreen::nextMenu() {
    return MT_NONE;
}

void LoadingScreen::show() {
    // Show background image etc

    if (isVisible) return;

    CEGUI::FrameWindow *loadingBackground = guiMgr->addStaticImage("Loading",0.5, 0.5,1.0, 1.0,"Loading","Loading");
    indicator = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","loadingIndicator"));
    //indicator->setFont("DroidSansMono-big.font");
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
    Root::getSingletonPtr()->renderOneFrame();
}

int LoadingScreen::getProgress() { return progress; }

void LoadingScreen::updateProgress(int progress) {
    std::stringstream out;
    out << progress << "%";
    indicator->setText(out.str());
    render();
    inputState->tick();
    if (inputState->isKeyDown(OIS::KC_ESCAPE)) std::exit(0);
}

void LoadingScreen::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Loading");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(indicator);  
}

bool LoadingScreen::end() { return false; }

bool LoadingScreen::visible() { return isVisible; }
