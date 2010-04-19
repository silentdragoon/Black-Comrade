#include "menuSystem.h"

MenuSystem::MenuSystem(SceneManager *sceneMgr, GuiManager *guiMgr,
                       InputState *inputState, RenderWindow *window)
    : sceneMgr(sceneMgr)
    , window(window)
    , inputState(inputState)
    , guiMgr(guiMgr)
    , currentMenuScreen(0)
    , fadingIn(true)
    , fadingOut(false)
{
    menuLoop = new StateUpdate();
}

void MenuSystem::init() {
    window->removeAllViewports();
    Camera *camera = sceneMgr->createCamera("menuCam");
    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));

    vp->update();
    CEGUI::System::getSingleton().setDefaultMouseCursor("Cursor", "Cursor");

    menuLoop->addTickable(inputState,"inputState");
}

void MenuSystem::shutdown() {
    inputState->clearKeyListener();
    inputState->clearMouseListener();
    sceneMgr->destroyCamera("menuCam");
    window->removeAllViewports();
}

void MenuSystem::run() {
    init();
    CEGUI::MouseCursor::getSingletonPtr()->show();
    inputState->addKeyListener(this);
    inputState->addMouseListener(this);
    menuLoop->startLoop();
}


void MenuSystem::render() {
    WindowEventUtilities weu = WindowEventUtilities();
    weu.messagePump();
    Root::getSingletonPtr()->renderOneFrame();
}

void MenuSystem::exit() {
    menuLoop->running = false;
}

bool MenuSystem::mouseMoved(const OIS::MouseEvent &e) {

    CEGUI::System& cegui = CEGUI::System::getSingleton();

    cegui.injectMouseMove(inputState->getMouseX(), inputState->getMouseY());
    cegui.injectMouseWheelChange(e.state.Z.rel * 0.03);

    return true;
}

bool MenuSystem::keyPressed(const OIS::KeyEvent &e) {
    // do event injection
    CEGUI::System& cegui = CEGUI::System::getSingleton();

    // key down
    cegui.injectKeyDown(e.key);

    // now character
    cegui.injectChar(e.text);


    return true;
}

bool MenuSystem::keyReleased(const OIS::KeyEvent &e) {
    CEGUI::System::getSingleton().injectKeyUp(e.key);
    return true;
}

bool MenuSystem::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().injectMouseButtonDown(convertOISButtonToCegui(id));

    return true;
}

bool MenuSystem::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().injectMouseButtonUp(convertOISButtonToCegui(id));

    return true;
}

CEGUI::MouseButton MenuSystem::convertOISButtonToCegui(int buttonID) {
   using namespace OIS;

   switch (buttonID) {
   case OIS::MB_Left:
        return CEGUI::LeftButton;
   case OIS::MB_Right:
        return CEGUI::RightButton;
   case OIS::MB_Middle:
        return CEGUI::MiddleButton;
    default:
        return CEGUI::LeftButton;
    }
}
