#include "preGame.h"

PreGame::PreGame(SceneManager *sceneMgr, Ogre::RenderWindow *window, InputState *inputState, 
                 GuiManager *guiMgr, NetworkingManager *networkingMgr)
    : MenuSystem(sceneMgr, guiMgr, inputState, window)
    , networkingMgr(networkingMgr)
{
    menuLoop->addTickable(this,"preGame");

    storyMenu = new StoryMenu(inputState,networkingMgr,guiMgr);
    networkRoleMenu = new NetworkRoleMenu(inputState,networkingMgr,guiMgr);
    gameRoleMenu = new GameRoleMenu(inputState,networkingMgr,guiMgr);
    loadingScreen = new LoadingScreen(inputState,guiMgr,networkingMgr);
}

CollaborationInfo* PreGame::showMenus() {

    currentMenuScreen = storyMenu;

    run();

    return networkingMgr->collabInfo;
}

void PreGame::waitForPlayers(CollaborationInfo *myInfo,
                             CollaborationInfo *pilotInfo,
                             CollaborationInfo *engineerInfo,
                             CollaborationInfo *navigatorInfo) {
    loadingScreen->setInfos(myInfo,pilotInfo,engineerInfo,navigatorInfo);
    loadingScreen->updateProgress(100);
    menuLoop->running = true;
    menuLoop->startLoop();
    shutdown();
}

LoadingScreen *PreGame::getLoadingScreen() {
    return loadingScreen;
}

void PreGame::tick() {
 
    if (currentMenuScreen) {
        if (currentMenuScreen->end()) {
            fadingIn = false;
            fadingOut = !guiMgr->fadeToBlack();
            if (!fadingOut) {
                // Hide + End it
                currentMenuScreen->hide();
                loadNextMenu();
            }
        } else if (!currentMenuScreen->visible()) {
            // Show it
            currentMenuScreen->show();
            fadingIn = !guiMgr->fadeFromBlack();
        } else if (currentMenuScreen->visible()) {
            if (!fadingIn) {
                // Process it
                currentMenuScreen->tick();
                if (currentMenuScreen == loadingScreen) {
                    if (loadingScreen->getProgress() == 0.0) {
                        exit();
                    }
                }
            }
        }	
    }
    if (fadingIn) {
        fadingIn = !guiMgr->fadeFromBlack();
    }
    else if (fadingOut) {
        fadingOut = !guiMgr->fadeToBlack();
    }
    render();
}

void PreGame::loadNextMenu() {

    switch (currentMenuScreen->nextMenu()) {
        case MT_CHOOSE_NETWORK_ROLE :
            currentMenuScreen = networkRoleMenu;
            break;
        case MT_CHOOSE_GAME_ROLE :
            currentMenuScreen = gameRoleMenu;
            break;
        case MT_LOADING :
            currentMenuScreen = loadingScreen;
            break;
        case MT_NONE :
            exit();
            break;
    }
}
