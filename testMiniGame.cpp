
#include "testMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>

TestMiniGame::TestMiniGame(KeyState *keyState)
    : isEnd(false)
    , keyState(keyState)
{
    
    std::cout << "Create\n";
    
    createScene();
}

void TestMiniGame::createScene()
{
    OverlayManager *om = OverlayManager::getSingletonPtr();

    TextAreaOverlayElement *texte;

    // Destroy old overlay if exists
    Overlay *existing;
    existing = om->getByName("testMiniGameOverlay");
    if(existing) om->destroy(existing);
    overlay = om->create("testMiniGameOverlay");

    OverlayContainer *oc = (OverlayContainer*)om->
        createOverlayElement("BorderPanel","test");
        
    texte = new TextAreaOverlayElement("");
    texte->setCaption("This is an overlay!");
    texte->setPosition(0.0f,0.0f);
    texte->setDimensions(0.0f,0.0f);
    texte->setColour(ColourValue::White);
    
    oc->addChild(texte);
    overlay->add2D(oc);
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
