
#include "fixMiniGame.h"

FixMiniGame::FixMiniGame()
{
    system = SS_NONE;
    isEnd = false;
}

void FixMiniGame::tick()
{
    switch(system) {
        case SS_NONE:
            system = SS_HULL;
            break;
        case SS_HULL:
            system = SS_ENGINES;
            break;
        case SS_ENGINES:
            system = SS_WEAPONS;
            break;
        case SS_WEAPONS:
            system = SS_SENSORS;
            break;
        case SS_SENSORS:
            isEnd = true;
            break;
    }
}

bool FixMiniGame::end()
{
    return isEnd;
}

int FixMiniGame::getScore()
{
    return 100;
}

ShipSystem FixMiniGame::getSystem()
{
    return system;
    std::cout << system << std::endl;
}

void FixMiniGame::returnKeyPressed() {}

FixMiniGame::~FixMiniGame() {}
