#include "gameTimer.h"

GameTimer::GameTimer(GameStateMachine *gameStateMachine)
    : gameStateMachine(gameStateMachine)
    , tickCount(0)
    , seconds(0)
{}

void GameTimer::tick() {
    GameState state = gameStateMachine->currentGameState();

    if (state != GS_TUTORIAL && state != GS_END && state != GS_GAME_OVER) {
        if (tickCount < 1 / ConstManager::getFloat("tick_period")) {
            tickCount ++;
        } else {
            tickCount = 0;
            seconds ++;
        }
    }
}
 
int GameTimer::getTime() { return seconds; }
