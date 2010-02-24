#include "consoleMiniGame.h"

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->append("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_2)) {
        isEnd = true;
        console->append("");
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
    console->append("---------------------------------------");
    console->append("BlackComrade System Repair v0.5 (beta)");
    console->append("---------------------------------------");
}

ConsoleMiniGame::~ConsoleMiniGame() {}
