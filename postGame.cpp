#include "postGame.h"

PostGame::PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
                   InputState *inputState,GuiManager *guiMgr,
                   SoundManager *soundMgr,
                   CollaborationInfo *pilotInfo,
                   CollaborationInfo *navInfo,
                   CollaborationInfo *engInfo)
    : sceneMgr(sceneMgr)
    , window(window)
    , inputState(inputState)
    , guiMgr(guiMgr)
    , soundMgr(soundMgr)
    , currentMenuScreen(0)
    , pilotInfo(pilotInfo)
    , navInfo(navInfo)
    , engInfo(engInfo)
{
    postGameLoop = new StateUpdate();

    postGameLoop->addTickable(soundMgr,"soundMgr");
    postGameLoop->addTickable(inputState,"inputState");
    postGameLoop->addTickable(this,"postGame");

    statsScreen = new StatsScreen(inputState,guiMgr,pilotInfo,navInfo,engInfo);
}

void PostGame::run() {

    guiMgr->destroyAllWindows();

    Camera *camera = sceneMgr->createCamera("postGameCam");
    window->removeAllViewports();
    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));

    vp->update();

    currentMenuScreen = statsScreen;

    postGameLoop->startLoop();
}

void PostGame::clearMenuUI() {

}

//void PreGame::showLoadingScreen() {
//    CEGUI::FrameWindow *loadingText = guiMgr->addStaticText("LoadingText", "Loading...",0.5, 0.5, 1);
//}

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

void PostGame::exit() {
    postGameLoop->running = false;
}
