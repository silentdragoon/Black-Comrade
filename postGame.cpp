#include "postGame.h"

PostGame::PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
                   InputState *inputState,GuiManager *guiMgr,
                   SoundManager *soundMgr,
                   CollaborationInfo *pilotInfo,
                   CollaborationInfo *navInfo,
                   CollaborationInfo *engInfo)
    : MenuSystem(sceneMgr,guiMgr,inputState,window)
    , soundMgr(soundMgr)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
{

    menuLoop->addTickable(this,"postGame");
    menuLoop->addTickable(soundMgr,"soundMgr");

    statsScreen = new StatsScreen(inputState,guiMgr,pilotInfo,navInfo,engInfo);
}

void PostGame::showMenus() {

    guiMgr->destroyAllWindows();
    currentMenuScreen = statsScreen;
    run();
}

void PostGame::tick() {

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
