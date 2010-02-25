
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
    float noscale = 1.6 / ratio;

    float wpixel = 1.0 / (float)winWidth;
    float hpixel = 1.0 / (float)winHeight;

    // Background Elements
    CEGUI::FrameWindow *xcrosshair  = guiManager->addStaticImage("XCrosshair",  0.5, 0.5,   0.05/ratio, 0.05,   "XCrosshair",   "XCross"  );
    CEGUI::FrameWindow *overlay     = guiManager->addStaticImage("Overlay",     0.5, 0.5,   1.6/ratio,  1.0,    "Overlay",      "Overlay" );

    // Foreground Elements

    // Crew Background
    CEGUI::FrameWindow *crewlb      = guiManager->addStaticImagePix("Crewl",        0.0,                 0.0,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );
    CEGUI::FrameWindow *crewrb      = guiManager->addStaticImagePix("Crewr",        1.0 - 104 * wpixel,  0.0,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );

    // Crew Avatars
    CEGUI::FrameWindow *crewav1     = guiManager->addStaticImagePix("Crewav1",      0.0 + 2 * wpixel,    0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Avatar1"  );
    CEGUI::FrameWindow *crewav2     = guiManager->addStaticImagePix("Crewav2",      1.0 - 102 * wpixel,  0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Avatar2"  );

    // Left
    CEGUI::FrameWindow *left        = guiManager->addStaticImagePix("Left",         0.0,                 1.0 - 263 * hpixel, 563 * wpixel, 263 * hpixel, "Left",        "Main"  );
}
