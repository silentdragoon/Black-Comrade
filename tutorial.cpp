#include "tutorial.h"

Tutorial::Tutorial(CollaborationInfo *tutee, GuiManager *guiMgr, 
                   HUD *hud, MiniGameManager *miniGameMgr)
    : tutee(tutee)
    , guiMgr(guiMgr)
    , hud(hud)
    , miniGameMgr(miniGameMgr)
    , state(TS_START)
{}

TutorialState Tutorial::getState() { return state; }

void Tutorial::tick() {
}
