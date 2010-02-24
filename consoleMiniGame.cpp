#include "consoleMiniGame.h"

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        std::cout << "score" << std::endl;
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_2)) {
        isEnd = true;
    }
}

bool ConsoleMiniGame::end() { return isEnd; }

int ConsoleMiniGame::getScore() {
    return score;
}
   
ConsoleMiniGame::ConsoleMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , isEnd(false)
    , score(0)
{
    std::cout << "New console minigame" << std::endl;
}

ConsoleMiniGame::~ConsoleMiniGame() {
    delete console;
}
