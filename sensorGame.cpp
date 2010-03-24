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
    , width(console->getWidth())
    , height(console->getHeight())
{
    char field[height][width];
    
    console->appendLine("========================================");
    console->appendLine("Sensor Minigame!");
    console->appendLine("========================================");
    console->appendLine("");
    console->appendLine(generateLine());
    console->appendLine(generateLine());
    console->appendLine(generateLine());
    console->appendLine("========================================");
}

char* SensorGame::generateLine() {
    int seed;
    char line[width];

    for (int i = 0; i < width; i++) {
        seed = rand() % 10;
        if (seed > 8) {
            line[i] = 'X';
        } else {
            line[i] = ' ';
        }
    }
    return line;
}

char* SensorGame::checkCollisions(char *line, int position) {
    if (line[position] == 'X') {
        line[position] = 'D';
        // death'd
    }
    return line;
}

void SensorGame::otherKeyPressed(const OIS::KeyEvent &arg) {
    if (begin) {
        
    }
    // for each (half? quarter? one?) second:
    // if left arrow is pressed, position--;
    // if right arrow is pressed, position++;
    
    // check for collisions
    
    // get next line, put '^' on current line if no collision occured
    
    
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

// console->clearPrompt();
// console->appendToPrompt(out.str());

