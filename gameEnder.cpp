#include "gameEnder.h"

GameEnder::GameEnder(GameStateMachine *stateMachine,
                     GuiManager *guiMgr, IExit *exit)
    : stateMachine(stateMachine)
    , guiMgr(guiMgr)
    , exit(exit)
    , ending(false)
{}

void GameEnder::tick() {
    if (stateMachine->currentGameState() == GS_GAME_OVER) {
        guiMgr->cutToBlack();
        ending = true;
    } else if (stateMachine->currentGameState() == GS_END) {
        ending = true;
    }
    if (ending) {
        if (!guiMgr->fadeToBlack(true)) {}
        else exit->exit();
    }
}
