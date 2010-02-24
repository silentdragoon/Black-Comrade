#include "consoleMiniGame.h"

void ConsoleMiniGame:: tick() {
    if(inputState->isKeyDown(OIS::KC_0)) {
        console->append("Score");
        score = score + 1;
    }
    if(inputState->isKeyDown(OIS::KC_2)) {
        inputState->clearKeyListener();
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
    inputState->addKeyListener(this);
    console->setVisible(true);
    console->append("---------------------------------------");
    console->append("BlackComrade System Repair v0.5 (beta)");
    console->append("---------------------------------------");
}

bool ConsoleMiniGame::keyPressed (const OIS::KeyEvent &arg) {
    std::cout << arg.key << std::endl;
    return true;
}

bool ConsoleMiniGame::keyReleased (const OIS::KeyEvent &arg) {
    return false;
}

ConsoleMiniGame::~ConsoleMiniGame() {}
