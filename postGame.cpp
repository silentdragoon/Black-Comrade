#include "postGame.h"

PostGame::PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window, InputState *inputState, 
        GuiManager *guiMgr, NetworkingManager *networkingMgr)
    : sceneMgr(sceneMgr)
    , window(window)
    , inputState(inputState)
    , guiMgr(guiMgr)
    , networkingMgr(networkingMgr)
    , currentMenuScreen(0)
{
    postGameLoop = new StateUpdate();

    Camera *camera = sceneMgr->createCamera("postGameCam");
    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));

    vp->update();

    postGameLoop->addTickable(inputState,"inputState");
    postGameLoop->addTickable(this,"postGame");

    statsScreen = 0;
}

void PostGame::run() {

    currentMenuScreen = statsScreen;

    postGameLoop->startLoop();

    window->removeAllViewports();
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
