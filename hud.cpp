
#include "hud.h"

HUD::HUD(GuiManager *guiManager, ShipState *shipState)
    : guiManager(guiManager)
    , shipState(shipState)
    , xCenter(0.5)
    , yCenter(0.47)
    , width(1)
    , height(1)
{
    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    float noscale = 1.6 / ratio;
    float g = 1;

    float wpixel = 1.0 / (float)winWidth * g;
    float hpixel = 1.0 / (float)winHeight * g;

    // Background Elements
    CEGUI::FrameWindow *xcrosshair  = guiManager->addStaticImage("XCrosshair",  0.5, 0.5,   0.05/ratio, 0.05,   "XCrosshair",   "XCross"  );
    //CEGUI::FrameWindow *overlay     = guiManager->addStaticImage("Overlay",     0.5, 0.5,   1.6/ratio,  1.0,    "Overlay",      "Overlay" );

    // Foreground Elements

    // Crew Background
    CEGUI::FrameWindow *crewlb      = guiManager->addStaticImagePix("Crewl",        0.0,                 0.0,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );
    CEGUI::FrameWindow *crewrb      = guiManager->addStaticImagePix("Crewr",        1.0 - 104 * wpixel,  0.0,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );

    // Crew Avatars
    CEGUI::FrameWindow *crewav1     = guiManager->addStaticImagePix("Crewav1",      0.0 + 2 * wpixel,    0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Avatar1"  );
    CEGUI::FrameWindow *crewav2     = guiManager->addStaticImagePix("Crewav2",      1.0 - 102 * wpixel,  0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Avatar2"  );

    // Left
    CEGUI::FrameWindow *left        = guiManager->addStaticImagePix("Left",         0.0,                 1.0 - 263 * hpixel, 563 * wpixel, 263 * hpixel, "Left",        "Main"  );


    // Progress Bars (failed to move these from guiManager, keeping them there for now
    // TODO: move progress bar code into this file

    shields = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarShe","shields"));
    sensors = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarSen","sensors"));
    weapons = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarWep","weapons"));
    engine = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarEng","engine"));
    hull = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarHul","hull"));

    guiManager->getRootWindow()->addChildWindow(shields);
    guiManager->getRootWindow()->addChildWindow(sensors);
    guiManager->getRootWindow()->addChildWindow(weapons);
    guiManager->getRootWindow()->addChildWindow(engine);
    guiManager->getRootWindow()->addChildWindow(hull);

    shields->setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 85) * hpixel,0)));
    sensors->setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 131) * hpixel,0)));
    weapons->setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 40) * hpixel,0)));
    engine-> setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 177) * hpixel,0)));
    hull->   setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 223) * hpixel,0)));
    shields->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));

    shields->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    sensors->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    weapons->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    engine->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    hull->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));

}

void HUD::setShields(float yeah) {
    shields->setProgress(yeah);
}

void HUD::setSensors(float yeah) {
    sensors->setProgress(yeah);
}

void HUD::setWeapons(float yeah) {
    weapons->setProgress(yeah);
}

void HUD::setEngines(float yeah) {
    engine->setProgress(yeah);
}

void HUD::setHull(float yeah) {
    hull->setProgress(yeah);
}
