
#include "hud.h"

HUD::HUD(GuiManager *guiManager, ShipState *shipState)
    : guiManager(guiManager)
    , shipState(shipState)
    , xCenter(0.5)
    , yCenter(0.47)
    , width(1)
    , height(1)
{

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;


    CEGUI::FrameWindow *xcrosshair = guiManager->addStaticImage("XCrosshair", 0.5, 0.5, 0.05/ratio, 0.05, "XCrosshair", "XCross");
    CEGUI::FrameWindow *overlay = guiManager->addStaticImage("Overlay", 0.5, 0.5, 1.6/ratio, 1.0, "Overlay", "Overlay");
}
