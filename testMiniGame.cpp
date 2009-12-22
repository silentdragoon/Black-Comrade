
#include "testMiniGame.h"
#include <iostream>

TestMiniGame::TestMiniGame()
    : isEnd(false)
{
    OverlayManager *om = OverlayManager::getSingletonPtr();
    overlay = om->create("testMiniGameOverlay");
}

void TestMiniGame::tick()
{
    std::cout << "Test\n";
}

bool TestMiniGame::end() 
{
    return isEnd;
}

Overlay *TestMiniGame::getOverlay()
{
    return overlay;
}
