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

    networkRoleMenu = new NetworkRoleMenu(inputState,networkingMgr,guiMgr);
    gameRoleMenu = new GameRoleMenu(inputState,networkingMgr,guiMgr);
    loadingScreen = new LoadingScreen(inputState,guiMgr,networkingMgr);

    CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    CEGUI::MouseCursor::getSingletonPtr()->show();
}

CollaborationInfo* PreGame::showMenus() {

    currentMenuScreen = networkRoleMenu;

    preGameLoop->startLoop();

    networkingMgr->lobby->chooseNick("Player");

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

void PreGame::handleMouse() {
    float  x = (float) inputState->getMouseX();
    float y = (float) inputState->getMouseY();
    CEGUI::System::getSingleton().injectMouseMove(x,y);
    std::cout << "Injecting mouse position..." << x << "," << y << "\n";
}

void PreGame::loadNextMenu() {

    if (currentMenuScreen == 0) {
        currentMenuScreen = networkRoleMenu;
        return;
    }

    switch (currentMenuScreen->nextMenu()) {
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
}
