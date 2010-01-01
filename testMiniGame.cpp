
#include "testMiniGame.h"
#include <iostream>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include <OgreFontManager.h>

TestMiniGame::TestMiniGame(KeyState *keyState, SceneManager *sceneManager)
    : isEnd(false)
    , keyState(keyState)
{
    
    std::cout << "Create\n";
    
    OverlayManager& overlayManager = OverlayManager::getSingleton();
      
    createScene();
    
    String s("testRect");
    
    Rect2D *rect = new Rect2D(s,1,1);
    //rect->setPosition(-0.5,-0.5);
    
    SceneNode *rectNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    rectNode->attachObject(rect);
    
    rectNode->setPosition(-1.0,-1.0,0.0);
    
    PlaneBoundedVolumeListSceneQuery *volQuery;
    
    volQuery = sceneManager->createPlaneBoundedVolumeQuery(PlaneBoundedVolumeList());
    
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
        createOverlayElement("Panel","test");
    
    oc->setDimensions(1.0f,1.0f);
    //oc->setMetricsMode(Ogre::GMM_PIXELS);
    
    texte = static_cast<TextAreaOverlayElement*>(
        om->createOverlayElement("TextArea","testPanel"));
    texte->setCaption("This is an overlay!");
    texte->setPosition(0.3f,0.45f);
    texte->setDimensions(1.0f,1.0f);
    texte->setCharHeight(0.1f);
    texte->setFontName("BlueBold");
    texte->setColourBottom(ColourValue::White);
    texte->setColourTop(ColourValue::White);
    
    PanelOverlayElement *panel;
    panel = static_cast<PanelOverlayElement*>(
        om->createOverlayElement("Panel","panel1"));
    panel->setPosition(0.1f,0.1f);
    panel->setDimensions(0.8f,0.8f);
    panel->setColour(ColourValue::Black);
    panel->setTransparent(false);
    //panel->setMaterialName("lambert1");
    
    oc->addChild(panel);
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
