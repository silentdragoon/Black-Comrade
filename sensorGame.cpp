#include "sensorGame.h"

SensorGame::SensorGame(Console *console, InputState *inputState)
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
    console->appendLine("Sensor Minigame!");
    console->appendLine("========================================");
    console->appendLine("");
    console->appendLine("Press as many keys as you can...");
    console->appendLine("Enter to begin repairs...");
}

void SensorGame::alphaNumKeyPressed(const OIS::KeyEvent &arg) {
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

void SensorGame::returnKeyPressed() {
    begin = true;
    console->appendLine("Go!");
}

void SensorGame::tick() {
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

ShipSystem SensorGame::getSystem() {
    return SS_SENSORS;
}

bool SensorGame::end() {
    return isEnd;
}

int SensorGame::getScore() {
    return score;
}

SensorGame::~SensorGame() {}
