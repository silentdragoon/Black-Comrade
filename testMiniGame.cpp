
#include "testMiniGame.h"
#include <iostream>

TestMiniGame::TestMiniGame(KeyState *keyState)
    : isEnd(false)
    , keyState(keyState)
{
    OverlayManager *om = OverlayManager::getSingletonPtr();
    std::cout << "Create\n";
    
    // Destroy old overlay if exists
    Overlay *existing;
    existing = om->getByName("testMiniGameOverlay");
    if(existing) om->destroy(existing);
    overlay = om->create("testMiniGameOverlay");
}

void TestMiniGame::tick()
{
    std::cout << "Test\n";
    
    if(keyState->isKeyDown(OIS::KC_2)) {
        isEnd = true;
    }
}

bool TestMiniGame::end() 
{
    return isEnd;
}

Overlay *TestMiniGame::getOverlay()
{
    return overlay;
}
