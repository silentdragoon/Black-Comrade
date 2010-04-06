#include "preGame.h"

PreGame::PreGame(SceneManager *sceneMgr, Ogre::RenderWindow *window, InputState *inputState, 
        GuiManager *guiMgr, NetworkingManager *networkingMgr)
    : sceneMgr(sceneMgr)
    , window(window)
    , inputState(inputState)
    , guiMgr(guiMgr)
    , networkingMgr(networkingMgr)
    , currentMenuScreen(0)
{
    preGameLoop = new StateUpdate();

    Camera *camera = sceneMgr->createCamera("preGameCam");
    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));

    vp->update();

    preGameLoop->addTickable(inputState,"inputState");
    preGameLoop->addTickable(this,"preGame");

    storyMenu = new StoryMenu(inputState,networkingMgr,guiMgr);
    networkRoleMenu = new NetworkRoleMenu(inputState,networkingMgr,guiMgr);
    gameRoleMenu = new GameRoleMenu(inputState,networkingMgr,guiMgr);
    loadingScreen = new LoadingScreen(inputState,guiMgr,networkingMgr);

    CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
}

CollaborationInfo* PreGame::showMenus() {

    currentMenuScreen = storyMenu;
    CEGUI::MouseCursor::getSingletonPtr()->show();
    inputState->addKeyListener(this);

    preGameLoop->startLoop();

    return networkingMgr->collabInfo;
}

LoadingScreen *PreGame::getLoadingScreen() {
    return loadingScreen;
}

void PreGame::hideLoadingScreen() {
    window->removeAllViewports();
    sceneMgr->destroyCamera("preGameCam");
    loadingScreen->hide();
}

void PreGame::clearMenuUI() {
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NetworkRoleMenu");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("GameRoleMenu");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("LoadingText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PilotRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("EngRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NavRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("ChosenRoleText");
}

void PreGame::render() {
    WindowEventUtilities weu = WindowEventUtilities();
    weu.messagePump();
    Root::getSingletonPtr()->renderOneFrame();
}

void PreGame::tick() {
 
   handleInput();

    if (currentMenuScreen) {
        if (currentMenuScreen->end()) {
            // Hide + End it
            currentMenuScreen->hide();
            loadNextMenu();
        } else if (!currentMenuScreen->visible()) {
            // Show it
            currentMenuScreen->show();
        } else {
            // Process it
            currentMenuScreen->tick();
        }	
    }
    render();
}

void PreGame::handleInput() {
    handleKeys();
    handleMouse();
}

void PreGame::handleKeys() {

}

bool PreGame::keyPressed(const OIS::KeyEvent &arg) {

    /*if (arg.key == OIS::KC_RETURN) {
        currentMenuScreen->returnKeyPressed();
        return true;
    } else if (arg.key == OIS::KC_BACK) {
        currentMenuScreen->backspaceKeyPressed();
        return true;
    } else if (arg.text == 0) {
        currentMenuScreen->otherKeyPressed(arg);
        return true;
    }

    char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890' ";
    for(int c=0;c<sizeof(legalchars);c++){
        if(legalchars[c]==arg.text){
            currentMenuScreen->alphaNumKeyPressed(arg);
            break;
        }
    }
    return true;*/
    CEGUI::System::getSingleton().injectKeyDown(arg.key);
    CEGUI::System::getSingleton().injectChar(arg.text);
}

bool PreGame::keyReleased(const OIS::KeyEvent &arg) { return false; }

void PreGame::handleMouse() {
    float  x = (float) inputState->getMouseX();
    float y = (float) inputState->getMouseY();
    CEGUI::System::getSingleton().injectMouseMove(x,y);
}

void PreGame::loadNextMenu() {

    switch (currentMenuScreen->nextMenu()) {
        case MT_CHOOSE_NETWORK_ROLE :
            currentMenuScreen = networkRoleMenu;
            break;
        case MT_CHOOSE_GAME_ROLE :
            currentMenuScreen = gameRoleMenu;
            break;
        case MT_NONE :
            // Start the game
            loadingScreen->show();
            render();
            exit();
            break;
    }
}

void PreGame::exit() {
    preGameLoop->running = false;
    inputState->clearKeyListener();
}
