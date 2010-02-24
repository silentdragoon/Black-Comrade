#include "consoleMiniGame.h"

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->append("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_2)) {
        isEnd = true;
        console->setVisible(false);
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
    console->setVisible(true);
    console->append("Welcome to the repair system");
}

ConsoleMiniGame::~ConsoleMiniGame() {}
