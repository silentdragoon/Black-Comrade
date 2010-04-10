#include "postGame.h"

PostGame::PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
                   InputState *inputState,GuiManager *guiMgr,
                   SoundManager *soundMgr,
                   CollaborationInfo *pilotInfo,
                   CollaborationInfo *navInfo,
                   CollaborationInfo *engInfo,
                   GameState finishState)
    : MenuSystem(sceneMgr,guiMgr,inputState,window)
    , soundMgr(soundMgr)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
{

    menuLoop->addTickable(this,"postGame");
    menuLoop->addTickable(soundMgr,"soundMgr");

    statsScreen = new StatsScreen(inputState,guiMgr,pilotInfo,navInfo,engInfo,finishState);
}

void PostGame::showMenus() {
    guiMgr->destroyAllWindows();
    guiMgr->cutToBlack();
    currentMenuScreen = statsScreen;
    run();
}

void PostGame::tick() {

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

void PostGame::loadNextMenu() {
    if (currentMenuScreen == 0) {
        currentMenuScreen = statsScreen;
        return;
    }

    switch (currentMenuScreen->nextMenu()) {
        case MT_NONE :
            // Properly exit the game
            exit();
            break;
    }
}
