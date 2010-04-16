#include "tutorial.h"

Tutorial::Tutorial(CollaborationInfo *tutee,
                   CollaborationInfo *tutee1, CollaborationInfo *tutee2, CollaborationInfo *tutee3,
                   GuiManager *guiMgr, HUD *hud, MiniGameManager *miniGameMgr, Door *door)
    : tutee(tutee)
    , tutee1(tutee1)
    , tutee2(tutee2)
    , tutee3(tutee3)
    , guiMgr(guiMgr)
    , hud(hud)
    , miniGameMgr(miniGameMgr)
    , state(TS_PRE)
    , door(door)
    , pauseProgress(0)
{}

TutorialState Tutorial::getState() { return state; }

void Tutorial::tick() {
    checkConsole();
    checkForCompletion();
    if (state == TS_END)
        door->open();
}

void Tutorial::checkConsole() {
    switch(state) {
        case(TS_PRE) :
            changeWithPause(TS_START,350);
            break;
        case(TS_START) :
            changeWithPause(TS_OPEN_CONSOLE,350);
            break;
        case(TS_OPEN_CONSOLE) :
            // We want the player to open the console
            if (miniGameMgr->hasConsoleBeenOpened())
                changeWithPause(TS_CLOSE_CONSOLE);
            break;
        case(TS_CLOSE_CONSOLE) :
            // We want the player to close the console
            if (miniGameMgr->hasConsoleBeenClosed())
               state = TS_WAITING_FOR_OTHERS;
            break;
    }
}

void Tutorial::changeWithPause(TutorialState newState, int pause) {
    if (pauseProgress > pause) {
        state = newState;
        pauseProgress = 0;
    } else {
        pauseProgress ++;
    }
}

void Tutorial::checkForCompletion() {
    if (state == TS_WAITING_FOR_OTHERS) {
        tutee->hasCompletedTutorial = true;
        bool completed = (tutee1->hasCompletedTutorial &&
                          tutee2->hasCompletedTutorial &&
                          tutee3->hasCompletedTutorial);
        if (completed) state = TS_END;
    }
}
