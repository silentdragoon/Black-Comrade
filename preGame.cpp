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
    loadingScreen = new LoadingScreen(inputState,guiMgr);
}

CollaborationInfo* PreGame::run() {

    currentMenuScreen = networkRoleMenu;

    preGameLoop->startLoop();

    networkingMgr->lobby->chooseNick("Player");

    window->removeAllViewports();
    sceneMgr->destroyCamera("preGameCam");

    return networkingMgr->collabInfo;
}

MapManager *PreGame::loadGame() {
    MapManager *mapMgr = new MapManager("examplemap_new.txt", sceneMgr);
    loadingScreen->hide();
    return mapMgr;
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

//void PreGame::showLoadingScreen() {
//    CEGUI::FrameWindow *loadingText = guiMgr->addStaticText("LoadingText", "Loading...",0.5, 0.5, 1);
//}

void PreGame::tick() {

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
            exit();
            break;
    }
}

void PreGame::exit() {
    preGameLoop->running = false;
}
