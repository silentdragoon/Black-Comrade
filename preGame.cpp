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

LoadingScreen *PreGame::getLoadingScreen() {
    return loadingScreen;
}

void PreGame::hideLoadingScreen() {
    window->removeAllViewports();
    shutdown();
    loadingScreen->hide();
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
                if (currentMenuScreen == loadingScreen) exit();
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
        case MT_NONE :
            currentMenuScreen = loadingScreen;
            break;
    }
}
