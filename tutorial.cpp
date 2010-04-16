#include "tutorial.h"

Tutorial::Tutorial(CollaborationInfo *tutee, GuiManager *guiMgr, 
                   HUD *hud, MiniGameManager *miniGameMgr, Door *door)
    : tutee(tutee)
    , guiMgr(guiMgr)
    , hud(hud)
    , miniGameMgr(miniGameMgr)
    , state(TS_START)
    , door(door)
{}

TutorialState Tutorial::getState() { return state; }

void Tutorial::tick() {
    checkConsole();
    if (state == TS_END) door->open();
}

void Tutorial::checkConsole() {
    switch(state) {
        case(TS_START) :
            state = TS_OPEN_CONSOLE;
            break;
        case(TS_OPEN_CONSOLE) :
            // We want the player to open the console
            if (miniGameMgr->hasConsoleBeenOpened())
                state = TS_CLOSE_CONSOLE;
            break;
        case(TS_CLOSE_CONSOLE) :
            // We want the player to close the console
            if (miniGameMgr->hasConsoleBeenClosed())
               state = TS_END;
            break;
    }
}
