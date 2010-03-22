#include "gameEnder.h"

GameEnder::GameEnder(GameStateMachine *stateMachine,
                     IExit *exit)
    : stateMachine(stateMachine)
    , exit(exit)
{}

void GameEnder::tick() {
    if (stateMachine->currentGameState() == GS_GAME_OVER) {
        exit->exit();
    } else if (stateMachine->currentGameState() == GS_END) {
        exit->exit();
    }
}
