
#include "hud.h"

HUD::HUD(GuiManager *guiManager, ShipState *shipState, GameRole gameRole)
    : guiManager(guiManager)
    , shipState(shipState)
    , gameRole(gameRole)
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
    CEGUI::FrameWindow *overlay     = guiManager->addStaticImage("Overlay",     0.5, 0.5,   1.6/ratio,  1.0,    "Overlay",      "Overlay" );

    // Foreground Elements

    // Crew Background
    CEGUI::FrameWindow *crewlb      = guiManager->addStaticImagePix("Crewl",        0.0 + 0 * wpixel,    0.0 + 0 * hpixel,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );
    CEGUI::FrameWindow *crewrb      = guiManager->addStaticImagePix("Crewr",        1.0 - 104 * wpixel,  0.0 + 0 * hpixel,                104 * wpixel, 157 * hpixel,  "Crew",        "EmptyBox" );

    // Crew Avatars

    if (gameRole==PILOT) {
    CEGUI::FrameWindow *crewav1     = guiManager->addStaticImagePix("Crewav2",      0.0 + 2 * wpixel,    0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav2"  );
    CEGUI::FrameWindow *crewav2     = guiManager->addStaticImagePix("Crewav3",      1.0 - 102 * wpixel,  0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav3"  );
    }
    if (gameRole==NAVIGATOR) {
    CEGUI::FrameWindow *crewav1     = guiManager->addStaticImagePix("Crewav1",      0.0 + 2 * wpixel,    0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav1"  );
    CEGUI::FrameWindow *crewav2     = guiManager->addStaticImagePix("Crewav3",      1.0 - 102 * wpixel,  0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav3"  );
    }
    if (gameRole==ENGINEER) {
    CEGUI::FrameWindow *crewav1     = guiManager->addStaticImagePix("Crewav1",      0.0 + 2 * wpixel,    0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav1"  );
    CEGUI::FrameWindow *crewav2     = guiManager->addStaticImagePix("Crewav2",      1.0 - 102 * wpixel,  0.0 + 2 * hpixel,   100 * wpixel, 100 * hpixel, "Crew",        "Crewav2"  );
    }




    // Left
    CEGUI::FrameWindow *left        = guiManager->addStaticImagePix("Left",         0.0,                 1.0 - 263 * hpixel, 563 * wpixel, 263 * hpixel, "Left",        "Main"  );

    // Right
    if(gameRole==PILOT)
    CEGUI::FrameWindow *rightPil    = guiManager->addStaticImagePix("RightPil",        1.0 - 411 * wpixel,  1.0 - 263 * hpixel, 411 * wpixel, 263 * hpixel, "RightPil",    "Main"  );
    if(gameRole==ENGINEER)
    CEGUI::FrameWindow *rightEng    = guiManager->addStaticImagePix("RightEng",        1.0 - 411 * wpixel,  1.0 - 263 * hpixel, 411 * wpixel, 263 * hpixel, "RightEng",    "Main"  );
    if(gameRole==NAVIGATOR)
    CEGUI::FrameWindow *rightNav    = guiManager->addStaticImagePix("RightNav",        1.0 - 411 * wpixel,  1.0 - 263 * hpixel, 411 * wpixel, 263 * hpixel, "RightNav",    "Main"  );

    // Slack Box

    status = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","status"));
    guiManager->getRootWindow()->addChildWindow(status);
    status->setSize(CEGUI::UVector2(CEGUI::UDim(80 * wpixel,0),CEGUI::UDim(20 * hpixel,0)));
    status->setPosition(CEGUI::UVector2(CEGUI::UDim(233 * wpixel,0),CEGUI::UDim(1 - 255 * hpixel,0)));

    // Transmission Log

    log = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","log"));
    guiManager->getRootWindow()->addChildWindow(log);
    log->setSize(CEGUI::UVector2(CEGUI::UDim(300 * wpixel,0),CEGUI::UDim(205 * hpixel,0)));
    log->setPosition(CEGUI::UVector2(CEGUI::UDim(9 * wpixel,0),CEGUI::UDim(1 - 218 * hpixel,0)));

    // Speed Indicator

    indicator = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","indicator"));
    guiManager->getRootWindow()->addChildWindow(indicator);
    indicator->setSize(CEGUI::UVector2(CEGUI::UDim(210 * wpixel,0),CEGUI::UDim(210 * hpixel,0)));
    indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(1 - 191 * wpixel,0),CEGUI::UDim(1 - 219 * hpixel,0)));

    // Progress Bars

    // Health

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


    shields->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    sensors->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    weapons->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    engine->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    hull->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));

    // Bars for power system stuff

    shieldRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarShe","shieldRate"));
    sensorRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarSen","sensorRate"));
    weaponRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarWep","weaponRate"));
    engineRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarEng","engineRate"));
    weaponCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarWep","weaponCharge"));
    // shieldCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarShe","shieldCharge"));

    shieldRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    sensorRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    weaponRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    engineRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));

    guiManager->getRootWindow()->addChildWindow(shieldRate);
    guiManager->getRootWindow()->addChildWindow(sensorRate);
    guiManager->getRootWindow()->addChildWindow(weaponRate);
    guiManager->getRootWindow()->addChildWindow(engineRate);
    guiManager->getRootWindow()->addChildWindow(weaponCharge);
    // guiManager->getRootWindow()->addChildWindow(shieldCharge);

    shieldRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 52) * wpixel,0),CEGUI::UDim(1.0 - 123 * hpixel,0)));
    weaponRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 6) * wpixel,0),CEGUI::UDim(1.0 - 123 * hpixel,0)));
    sensorRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 98) * wpixel,0),CEGUI::UDim(1.0 - 123 * hpixel,0)));
    engineRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 142) * wpixel,0),CEGUI::UDim(1.0 - 123 * hpixel,0)));
    weaponCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 6) * wpixel,0),CEGUI::UDim(1.0 - 228 * hpixel,0)));
    // shieldCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 127) * wpixel,0),CEGUI::UDim(1.0 - 228 * hpixel,0)));

    shieldRate->setSize(CEGUI::UVector2(CEGUI::UDim(34 * wpixel,0),CEGUI::UDim(120 * hpixel,0)));
    sensorRate->setSize(CEGUI::UVector2(CEGUI::UDim(34 * wpixel,0),CEGUI::UDim(120 * hpixel,0)));
    weaponRate->setSize(CEGUI::UVector2(CEGUI::UDim(34 * wpixel,0),CEGUI::UDim(120 * hpixel,0)));
    engineRate->setSize(CEGUI::UVector2(CEGUI::UDim(34 * wpixel,0),CEGUI::UDim(120 * hpixel,0)));
    weaponCharge->setSize(CEGUI::UVector2(CEGUI::UDim(60 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    // shieldCharge->setSize(CEGUI::UVector2(CEGUI::UDim(60 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
}

void HUD::setStatus(std::string stat) {
    status->setText(stat);
}

void HUD::setLog(std::string wang) {
    log->setText(wang);
}

void HUD::setSpeedIndicator(std::string giraffe) {
    indicator->setText(giraffe);
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

void HUD::setShieldRate(float yeah) {
    shieldRate->setProgress(yeah);
}

void HUD::setSensorRate(float yeah) {
    sensorRate->setProgress(yeah);
}


void HUD::setWeaponRate(float what) {
    weaponRate->setProgress(what);
}


void HUD::setEngineRate(float alright) {
    engineRate->setProgress(alright);
}

void HUD::setWeaponCharge(float yeah) {
    weaponCharge->setProgress(yeah);
}
