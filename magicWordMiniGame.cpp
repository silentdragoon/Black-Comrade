#include "magicWordMiniGame.h"

MagicWordMiniGame::MagicWordMiniGame(Console *console, InputState *inputState)
    : console(console)
    , inputState(inputState)
    , ticks(0)
{}

void MagicWordMiniGame::tick() {
    if (ticks > 10) {
        console->appendLine("YOU DIDN'T SAY THE MAGIC WORD!");
        ticks = 0;
    }
    ticks++;
}

bool MagicWordMiniGame::end() {
    return false;
}

int MagicWordMiniGame::getScore() {
    return 0;
}

MagicWordMiniGame::~MagicWordMiniGame() {}
