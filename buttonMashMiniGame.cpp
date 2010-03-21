#include "buttonMashMiniGame.h"

ButtonMashMiniGame::ButtonMashMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , score(0)
    , dscore(0.0)
    , ticks(0)
    , tickLimit(1000)
    , broke(0)
    , begin(false)
    , isEnd(false)
{
    console->appendLine("========================================");
    console->appendLine("Some other tool (beta)");
    console->appendLine("========================================");
    console->appendLine("");
    console->appendLine("Press as many keys as you can...");
    console->appendLine("Enter to begin repairs...");
}

void ButtonMashMiniGame::alphaNumKeyPressed(const OIS::KeyEvent &arg) {
    if (begin) {
        dscore = dscore + 0.1;
        broke ++;
        //score = (int) floor(dscore);
        std::stringstream out;
        out << broke;
        console->clearPrompt();
        console->appendToPrompt(out.str());
    }
}

void ButtonMashMiniGame::returnKeyPressed() {
    begin = true;
    console->appendLine("Go!");
}

void ButtonMashMiniGame::tick() {
    if (!begin) return;

    score = 0;
    ticks++;
    if (ticks > tickLimit) {
        isEnd = true;
        std::stringstream out;
        out << "Final score: " << broke;
        console->appendLine(out.str());
    }
    if (dscore > 1) {
        dscore = 0;
        score = 1;
    }
}

ShipSystem ButtonMashMiniGame::getSystem() {
    return SS_WEAPONS;
}

bool ButtonMashMiniGame::end() {
    return isEnd;
}

int ButtonMashMiniGame::getScore() {
    return score;
}

ButtonMashMiniGame::~ButtonMashMiniGame() {}
