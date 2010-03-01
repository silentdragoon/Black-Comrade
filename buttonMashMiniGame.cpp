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
        score = (int) floor(dscore);
        std::string s;
        std::stringstream out;
        out << broke;
        console->clearPrompt();
        console->appendToPrompt(out.str());
    }
}

void ButtonMashMiniGame::returnKeyPressed() {
    begin = true;
}

void ButtonMashMiniGame::tick() {
    if (!begin) return;
    ticks++;
    if (ticks > tickLimit) isEnd = true;

}

ShipSystem ButtonMashMiniGame::getSystem() {
    return SS_ENGINES;
}

bool ButtonMashMiniGame::end() {
    return isEnd;
}

int ButtonMashMiniGame::getScore() {
    return score;
}

ButtonMashMiniGame::~ButtonMashMiniGame() {}
