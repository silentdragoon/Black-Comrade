
#include "hud.h"

HUD::HUD(GuiManager *guiManager, ShipState *shipState, GameRole gameRole, MapManager *mapMgr)
    : guiManager(guiManager)
    , shipState(shipState)
    , gameRole(gameRole)
    , mapMgr(mapMgr)
    , xCenter(0.5)
    , yCenter(0.47)
    , width(1)
    , height(1)
    , prevX(10000)
    , prevY(10000)
    , controlsBeenShown(false)
    , largeMapBeenShown(false)
    , largeRadarBeenShown(false)
{
    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    ratio = winWidth / (float)winHeight;
    float noscale = 1.6 / ratio;
    float g = (1.0*winWidth)/1680.0;

    wpixel = 1.0 / (float)winWidth * g;
    hpixel = 1.0 / (float)winHeight * g;

    // Make common HUD
    makeCommonHUD();

    // Make player dependent HUD
    switch (gameRole) {
        case PILOT:
            makePilotHUD();
            break;
        case NAVIGATOR:
            makeNavigatorHUD();
            break;
        case ENGINEER:
            makeEngineerHUD();
            break;
    }

    // Hide the controls until we need them
    controls->hide();
    controls->setAlwaysOnTop(true);

    // TEST	
    // CEGUI::FrameWindow *test     = guiManager->addStaticImage("ScoresFail",     0.5, 0.5,   1680 * wpixel,  1050 * hpixel,    "ScoresFail",      "Whole" );

}

bool HUD::hasControlScreenBeenShown() { return controlsBeenShown; }

bool HUD::hasLargeMapBeenShown() { return largeRadarBeenShown; }

bool HUD::hasLargeRadarBeenShown() { return largeMapBeenShown; }

void HUD::showKeyHint(KeyHint keyHint) {
    hideKeyHint();

    // TODO: For each case, set the image of the key hint
    //       window to an image of the key

    switch (keyHint) {
        case KH_F1:
            break;
        case KH_TAB:
            break;
        case KH_ESCAPE:
            break;
    }

    // TODO: Set the key hint window to visible
}

void HUD::hideKeyHint() {
    // TODO: Set the key hint window to hidden
}

void HUD::highlightElement(HUDElement element) {
    // TODO: For each case, swap the image of the element
    //       to a highlighted version

    stopHighlightingAllElements();

    switch (element) {
        case HE_AVATARS:
            break;
        case HE_HEALTH_BARS:
            std::cout << "FLASH HEALTH BARS\n";
            break;
        case HE_CHARGE_BARS:
            break;
        case HE_POWER_BARS:
            break;
        case HE_MINI_MAP:
            break;
        case HE_MINI_RADAR:
            break;
        case HE_MISSION_LOG:
            break;
    }
}

void HUD::stopHighlightingElement(HUDElement element) {
    // TODO: For each case, swap the image of the element
    //       to the normal version

    switch (element) {
        case HE_AVATARS:
            break;
        case HE_HEALTH_BARS:
            break;
        case HE_CHARGE_BARS:
            break;
        case HE_POWER_BARS:
            break;
        case HE_MINI_MAP:
            break;
        case HE_MINI_RADAR:
            break;
        case HE_MISSION_LOG:
            break;
    }
}

void HUD::stopHighlightingAllElements() {
    // TODO:: Show the normal version of all images
}

void HUD::makeCommonHUD() {

    // Overlay
    //guiManager->addStaticImage("Overlay", 0.5, 0.5, 1.6/ratio, 1.0, "Overlay", "Overlay");

    // Crosshair
    crosshair = guiManager->addStaticImage("XCrosshair",  0.5, 0.5, 0.05/ratio, 0.05, "XCrosshair", "XCross");

    // Weapon & Shield Charge backgrounds
    
    guiManager->addStaticImagePix("Charge", 1.0 - 48 *wpixel, 0.4 - 300 * hpixel, 48 * wpixel, 600 * hpixel, "Charge", "hole"  );
    //guiManager->addStaticImagePix("Charges", 1.0 - 48*wpixel,  0.4 + 80 * hpixel, 48 * wpixel, 240 * hpixel, "Charges", "Whole");

    // Left box
    guiManager->addStaticImagePix("Left",  0.0, 1.0 - 229 * hpixel, 566 * wpixel, 229 * hpixel, "Left", "Main");
    
    // Have to do this here as it's a background element...
    
    switch (gameRole) {
        case PILOT:
			guiManager->addStaticImagePix("Crewl", 0 * wpixel, 0.4 - 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "NavBox" );
			guiManager->addStaticImagePix("Crewr", 0 * wpixel, 0.4 + 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "EngBox" );
			guiManager->addStaticImagePix("Crews", 0 * wpixel, 0.4 - 300 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "PilBox" );
            break;
        case NAVIGATOR:
        	guiManager->addStaticImagePix("Crewl", 0 * wpixel, 0.4 - 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "PilBox" );
			guiManager->addStaticImagePix("Crewr", 0 * wpixel, 0.4 + 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "EngBox" );
			guiManager->addStaticImagePix("Crews", 0 * wpixel, 0.4 - 300 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "NavBox" );
            break;
        case ENGINEER:
			guiManager->addStaticImagePix("Crewl", 0 * wpixel, 0.4 - 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "PilBox" );
			guiManager->addStaticImagePix("Crewr", 0 * wpixel, 0.4 + 100 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "NavBox" );
			guiManager->addStaticImagePix("Crews", 0 * wpixel, 0.4 - 300 * hpixel, 104 * wpixel, 200 * hpixel,  "Crew", "EngBox" );
            break;
    }
    

    // Teammate Info
    
    teaminfo1 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","teaminfo1"));
    teaminfo2 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","teaminfo2"));
    teaminfo3 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","teaminfo3"));
    guiManager->getRootWindow()->addChildWindow(teaminfo1);
    guiManager->getRootWindow()->addChildWindow(teaminfo2);
    guiManager->getRootWindow()->addChildWindow(teaminfo3);
    teaminfo1->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    teaminfo2->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    teaminfo3->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    teaminfo1->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 - (100 - 142) * hpixel,0)));
    teaminfo2->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 + (100 + 142) * hpixel,0)));
    teaminfo3->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 - (300 - 142) * hpixel,0)));
    
    // Kills
    
    kills1 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","kills1"));
    kills2 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","kills2"));
	kills3 = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","kills3"));
    guiManager->getRootWindow()->addChildWindow(kills1);
    guiManager->getRootWindow()->addChildWindow(kills2);
    guiManager->getRootWindow()->addChildWindow(kills3);
    kills1->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    kills2->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    kills3->setSize(CEGUI::UVector2(CEGUI::UDim(94 * wpixel,0),CEGUI::UDim(21 * hpixel,0)));
    kills1->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 - (100 - 163) * hpixel,0)));
    kills2->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 + (100 + 163) * hpixel,0)));
    kills3->setPosition(CEGUI::UVector2(CEGUI::UDim(5 * wpixel,0),CEGUI::UDim(0.4 - (300 - 163) * hpixel,0)));
    
    // Slack Box

    status = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","status"));
    guiManager->getRootWindow()->addChildWindow(status);
    status->setSize(CEGUI::UVector2(CEGUI::UDim(80 * wpixel,0),CEGUI::UDim(20 * hpixel,0)));
    status->setPosition(CEGUI::UVector2(CEGUI::UDim(233 * wpixel,0),CEGUI::UDim(1 - 255 * hpixel,0)));

    // Transmission Log

    log = static_cast<CEGUI::MultiLineEditbox*>(guiMgr->createWindow("BlackComrade/MultiLineEditbox","log"));
    guiManager->getRootWindow()->addChildWindow(log);
    log->setSize(CEGUI::UVector2(CEGUI::UDim(300 * wpixel,0),CEGUI::UDim(205 * hpixel,0)));
    log->setPosition(CEGUI::UVector2(CEGUI::UDim(9 * wpixel,0),CEGUI::UDim(1 - 218 * hpixel,0)));
    log->setWordWrapping(true);

    // Health

    //shields = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarShe","shields"));
    sensors = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarSen","sensors"));
    weapons = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarWep","weapons"));
    engine = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarEng","engine"));
    hull = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarHul","hull"));

    //guiManager->getRootWindow()->addChildWindow(shields);
    guiManager->getRootWindow()->addChildWindow(sensors);
    guiManager->getRootWindow()->addChildWindow(weapons);
    guiManager->getRootWindow()->addChildWindow(engine);
    guiManager->getRootWindow()->addChildWindow(hull);

    //shields->setPosition(CEGUI::UVector2(CEGUI::UDim(440 * wpixel,0),CEGUI::UDim(1 - (263 - 85) * hpixel,0)));
    sensors->setPosition(CEGUI::UVector2(CEGUI::UDim(443 * wpixel,0),CEGUI::UDim(1 - (229 - 97) * hpixel,0)));
    weapons->setPosition(CEGUI::UVector2(CEGUI::UDim(443 * wpixel,0),CEGUI::UDim(1 - (229 - 51) * hpixel,0)));
    engine-> setPosition(CEGUI::UVector2(CEGUI::UDim(443 * wpixel,0),CEGUI::UDim(1 - (229 - 144) * hpixel,0)));
    hull->   setPosition(CEGUI::UVector2(CEGUI::UDim(443 * wpixel,0),CEGUI::UDim(1 - (229 - 190) * hpixel,0)));

    //shields->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    sensors->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    weapons->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    engine->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    hull->setSize(CEGUI::UVector2(CEGUI::UDim(120 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));

    // Weapon and Shield Charge
    
    weaponCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarWep","weaponCharge"));
    shieldCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarShe","shieldCharge"));
      
    weaponCharge->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    shieldCharge->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        
    guiManager->getRootWindow()->addChildWindow(weaponCharge);
    guiManager->getRootWindow()->addChildWindow(shieldCharge);
        
    weaponCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - 40*wpixel,0),CEGUI::UDim(0.4 - (300 - 40) * hpixel,0)));
    shieldCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - 40*wpixel,0),CEGUI::UDim(0.4 - (300 - 340)  * hpixel,0)));
        
    weaponCharge->setSize(CEGUI::UVector2(CEGUI::UDim(32 * wpixel,0),CEGUI::UDim(252 * hpixel,0)));
    shieldCharge->setSize(CEGUI::UVector2(CEGUI::UDim(32 * wpixel,0),CEGUI::UDim(252 * hpixel,0)));

    // Status Indicator
    
    statusIndicatorsStealth = guiManager->addStaticImage("StatusIndicators", 0.5, 0.0 + 23*hpixel, 206 * wpixel, 46 * hpixel,
                                                         "StatusIndicators", "Stealth");

    statusIndicatorsSwarms = guiManager->addStaticImage("StatusIndicators2", 0.5, 0.0 + 23*hpixel, 206 * wpixel, 46 * hpixel,
                                                        "StatusIndicators", "Swarms");

    statusIndicatorsComrade = guiManager->addStaticImage("StatusIndicators3", 0.5, 0.0 + 23*hpixel, 206 * wpixel, 46 * hpixel,
                                                         "StatusIndicators", "BlackComrade");

    statusIndicatorsBossHealth = guiManager->addStaticImage("StatusIndicators4", 0.5, 0.0 + 23*hpixel, 206 * wpixel, 46 * hpixel,
                                                            "StatusIndicators", "BossHealthbar");
    
    statusIndicatorsBlank = guiManager->addStaticImage("StatusIndicators5", 0.5, 0.0 + 23*hpixel, 206 * wpixel, 46 * hpixel,
                                                       "StatusIndicators", "Blank");    
    
    statusIndicatorsStealth->setVisible(false);
    statusIndicatorsSwarms->setVisible(false);
    statusIndicatorsComrade->setVisible(false);
    statusIndicatorsBossHealth->setVisible(false);
    statusIndicatorsBlank->setVisible(false);
    
    // Countdown timer       
            
    countdown = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","countdown"));
    //countdown->setFont("DroidSansMono-big.font");
    guiManager->getRootWindow()->addChildWindow(countdown);
    countdown->setSize(CEGUI::UVector2(CEGUI::UDim(206 * wpixel,0),CEGUI::UDim(46 * hpixel,0)));
    countdown->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 103 * wpixel,0),CEGUI::UDim(0 + 0 * hpixel,0)));
    countdown->setVisible(false);

    // Boss Health
    
    bossHealthbar = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarComrade","Comrade"));
    guiManager->getRootWindow()->addChildWindow(bossHealthbar);
    bossHealthbar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 7*wpixel,0),CEGUI::UDim(0.0 + 6 * hpixel,0)));
    bossHealthbar->setSize(CEGUI::UVector2(CEGUI::UDim(106 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
    bossHealthbar->setVisible(false);
    
    // Boss Text
    
    bossText = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","bossText"));
    guiManager->getRootWindow()->addChildWindow(bossText);
    bossText->setSize(CEGUI::UVector2(CEGUI::UDim(196 * wpixel,0),CEGUI::UDim(46 * hpixel,0)));
    bossText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 93 * wpixel,0),CEGUI::UDim(0 + 0 * hpixel,0)));
    bossText->setText("Boss HP:");
    bossText->setVisible(false);
}

void HUD::makePilotHUD() {

    // Crew avatars

    guiManager->addStaticImagePix("Crewav2", 2 * wpixel, 0.4 - (100 - 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav2");
    guiManager->addStaticImagePix("Crewav3", 2 * wpixel, 0.4 + (100 + 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav3");
    guiManager->addStaticImagePix("Crewav1", 2 * wpixel, 0.4 - (300 - 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav1");


    // Repair Avatars

    repairAv1 = guiManager->addStaticImagePix("Repairav2", 2 * wpixel, 0.4 - (100 - 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav2");
    repairAv2 = guiManager->addStaticImagePix("Repairav3", 2 * wpixel, 0.4 + (100 + 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav3");
    repairAv3 = guiManager->addStaticImagePix("Repairav1", 2 * wpixel, 0.4 - (100 - 38) * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav1");


    repairAv1->setVisible(false);
    repairAv2->setVisible(false);
    repairAv3->setVisible(false);
    


    // Right box
    guiManager->addStaticImagePix("RightPil", 1.0 - 138 * wpixel,  1.0 - 75 * hpixel,
                                  138 * wpixel, 75 * hpixel, "RightPil", "Main");

    // Speed indicator
    /*
    indicator = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","indicator"));
    //indicator->setFont("DroidSansMono-big.font");
    guiManager->getRootWindow()->addChildWindow(indicator);
    indicator->setSize(CEGUI::UVector2(CEGUI::UDim(100 * wpixel,0),CEGUI::UDim(50 * hpixel,0)));
    indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(1 - 99 * wpixel,0),CEGUI::UDim(1 - 60 * hpixel,0)));
	*/
	
	// Speedo
	
	speedo0 = guiManager->addStaticImagePix("Speedo0", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo0");
	speedo1 = guiManager->addStaticImagePix("Speedo1", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo1");
	speedo2 = guiManager->addStaticImagePix("Speedo2", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo2");
	speedo3 = guiManager->addStaticImagePix("Speedo3", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo3");
	speedo4 = guiManager->addStaticImagePix("Speedo4", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo4");
	speedo5 = guiManager->addStaticImagePix("Speedo5", 1.0 - 129 * wpixel, 1.0 - 68 * hpixel, 124 * wpixel, 60 * hpixel, "Speedo", "Speedo5");
	
	speedo1->setVisible(false);
	speedo2->setVisible(false);
	speedo3->setVisible(false);
	speedo4->setVisible(false);
	speedo5->setVisible(false);

    // Controls
    controls = guiManager->addStaticImage("KeyboardPilot",0.5, 0.5,1.0, 1.0,"KeyboardPilot","Loading");
}

void HUD::makeNavigatorHUD() {
    // Crew avatars
    guiManager->addStaticImagePix("Crewav1", 2 * wpixel, 0.3 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav1");
    guiManager->addStaticImagePix("Crewav3", 2 * wpixel, 0.6 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav3");
    guiManager->addStaticImagePix("Crewav2", 2 * wpixel, 0.0 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav2");

    // Repair avatars

    repairAv1 = guiManager->addStaticImagePix("Repairav1", 2 * wpixel, 0.3 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav1");
    repairAv2 = guiManager->addStaticImagePix("Repairav3", 2 * wpixel, 0.6 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav3");
    repairAv3 = guiManager->addStaticImagePix("Repairav2", 2 * wpixel, 0.0 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav2");

    repairAv1->setVisible(false);
    repairAv2->setVisible(false);
    repairAv3->setVisible(false);

    // Right box
    guiManager->addStaticImagePix("RightNav", 1.0 - 229 * wpixel,  1.0 - 229 * hpixel,
                                  229 * wpixel, 229 * hpixel, "RightNav", "Main");

    // Minimap
    fullmap = buildFullMap();
    guiManager->getRootWindow()->addChildWindow(fullmap);
    fullmap->setVisible(false);

    // Create the minimap
    minimap = buildMiniMap(0);
    guiManager->getRootWindow()->addChildWindow(minimap);

    // Controls
    controls = guiManager->addStaticImage("KeyboardNavigator",0.5, 0.5,1.0, 1.0,"KeyboardNavigator","Loading");
}

void HUD::makeEngineerHUD() {
    // Crew avatars
    guiManager->addStaticImagePix("Crewav1", 2 * wpixel, 0.3 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav1");
    guiManager->addStaticImagePix("Crewav2", 2 * wpixel, 0.6 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav2");
    guiManager->addStaticImagePix("Crewav3", 2 * wpixel, 0.0 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Crewav3");

    // Repair Avatars

    repairAv1 = guiManager->addStaticImagePix("Repairav1", 2 * wpixel, 0.3 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav1");
    repairAv2 = guiManager->addStaticImagePix("Repairav2", 2 * wpixel, 0.6 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav2");
    repairAv3 = guiManager->addStaticImagePix("Repairav3", 2 * wpixel, 0.0 + 2 * hpixel, 100 * wpixel, 100 * hpixel, "Crew", "Repairav3");

    repairAv1->setVisible(false);
    repairAv2->setVisible(false);
    repairAv3->setVisible(false);

    // Right box
    guiManager->addStaticImagePix("RightEng", 1.0 - 367 * wpixel,  1.0 - 230 * hpixel,
                                  367 * wpixel, 230 * hpixel, "RightEng",    "Main"  );

    // Power bars
    shieldRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarShe","shieldRate"));
    weaponRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarWep","weaponRate"));
    engineRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarEng","engineRate"));

    shieldRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    weaponRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
    engineRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));

    guiManager->getRootWindow()->addChildWindow(shieldRate);
    guiManager->getRootWindow()->addChildWindow(weaponRate);
    guiManager->getRootWindow()->addChildWindow(engineRate);

    shieldRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 52 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68)* hpixel,0)));
    weaponRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 5 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68) * hpixel,0)));
    engineRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 98 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68) * hpixel,0)));

    shieldRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));
    weaponRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));
    engineRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));

    // Controls
    controls = guiManager->addStaticImage("KeyboardEngineer",0.5, 0.5,1.0, 1.0,"KeyboardEngineer","Loading");
}

void HUD::appendTileEnding(std::stringstream &ss, int xpos, int ypos, int rotate)
{
    rotate = rotate < 0 ? -rotate : rotate;

    bool sides[4];

    for(int c = 1; c <= 4; ++c) {
        if(mapMgr->mts[xpos][ypos]->getAdjacent(c)!=0) {
            sides[((c + rotate - 1) % 4) + 1] = true;
        } else { sides[((c + rotate - 1) % 4) + 1] = false; }
    }
    
    for(int c = 1; c <= 4; ++c) {
        if(sides[c]) ss << '-' << c;
    }
}

CEGUI::FrameWindow* HUD::buildFullMap() {
    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("fullMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;

    // GET MAP PARTS HERE WITH A LOOOOOOP
    // -------------------------------------------------------------------
    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));

    for(int xpos=0;xpos<Const::MAPSIZE;xpos++) {
        for(int ypos=0;ypos<Const::MAPSIZE;ypos++) {
            CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
            if(mapMgr->mts[xpos][ypos]->isEmpty()) {
                ic.setImage("Minimap","mapTile-blank");
            } else if(mapMgr->mts[xpos][ypos]->isObj()) {
                ic.setImage("MinimapNow","mapTileN-special");
            } else {
                std::stringstream tile;
                if((xpos==x)&&(ypos==y)) {
                    tile << "mapTileN";
                } else if (mapMgr->mts[xpos][ypos]->isStart()) {
                    tile << "mapTileS";
                } else if (mapMgr->mts[xpos][ypos]->isEnd()) {
                    tile << "mapTileE";
                } else {
                    tile << "mapTile";
                }
                
                appendTileEnding(tile, xpos, ypos, 0);
                
                string name = tile.str();
                if((xpos==x)&&(ypos==y)) {
                    ic.setImage("MinimapNow",name);
                } else if (mapMgr->mts[xpos][ypos]->isStart()) {
                    ic.setImage("MinimapStart",name);
                } else if (mapMgr->mts[xpos][ypos]->isEnd()) {
                    ic.setImage("MinimapEnd",name);
                } else {
                    ic.setImage("Minimap",name);
                }
            }
            ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
            ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

            CEGUI::ComponentArea ca = CEGUI::ComponentArea();

            ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(xpos*0.015/ratio,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
            ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(ypos*0.015,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
            ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(0.015/ratio,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
            ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(0.015,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);

            ic.setComponentArea(ca);

            is.addImageryComponent(ic);
        }
    }

    // END LOOP SECTION
    // ------------------------------------------------------------------
    
    lookFeel.addImagerySection(is);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(1);
    CEGUI::SectionSpecification ss = CEGUI::SectionSpecification("fullMap","enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    // TODO:: Almost certain some of these should be constants
    // Create the FrameWindow to return
    CEGUI::FrameWindow *fullmap = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair"));
    fullmap->setLookNFeel(lookFeel.getName());
    fullmap->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5-30*0.015/ratio/2,0),CEGUI::UDim(0.5-30*0.015/2,0)));
    fullmap->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(1.0f,0)));

    return fullmap;
}

CEGUI::FrameWindow* HUD::buildMiniMap(int rotate) {

    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("miniMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    
    float g = (1.0*winWidth)/1680.0;
    float wpixel = 1.0 / (float)winWidth * g;
    float hpixel = 1.0 / (float)winHeight * g;

    // Center of minimap in map
    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));
    prevX = x;
    prevY = y;
    
    // Position in minimap
    int mapx = 1;
    int mapy = 1;

    int i=0;

    // Offset from center of minimap
    int offsetx = 0;
    int offsety = 0;

    while(i<9) {
        if(i==1) {
            offsetx = -1; 
            offsety = -1; 
            mapx = 0; 
            mapy = 0;
        }
        if(i==2) {
            offsetx = 0;
            offsety = -1;
            mapx = 1;
            mapy = 0;
        }
        if(i==3) {
            offsetx = +1;
            offsety = -1;
            mapx = 2;
            mapy = 0;
        }
        if(i==4) {
            offsetx = -1;
            offsety = 0;
            mapx = 0;
            mapy = 1;
        }
        if(i==5) {
            offsetx = +1;
            offsety = 0;
            mapx = 2;
            mapy = 1;
        }
        if(i==6) {
            offsetx = -1;
            offsety = +1;
            mapx = 0;
            mapy = 2;
        }
        if(i==7) {
            offsetx = 0;
            offsety = +1;
            mapx = 1;
            mapy = 2;
        }
        if(i==8) {
            offsetx = +1;
            offsety = +1;
            mapx = 2;
            mapy = 2;
        }

        // Position of tile within whole map
        int xpos;
        int ypos;
        
        // cout << "ROATAATATAT " << rotate << endl;
        
        // Rotation
        switch(rotate){
            case 1:
                xpos = x + offsety;
                ypos = y - offsetx;
                break;
            case 2:
                xpos = x - offsetx;
                ypos = y - offsety;
                break;
            case 3:
                xpos = x - offsety;
                ypos = y + offsetx;
                break;
            default:
                xpos = x + offsetx;
                ypos = y + offsety;
        }

        CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
        if(((xpos<0)||(ypos<0))||((xpos>Const::MAPSIZE)||(ypos>Const::MAPSIZE))) {
            ic.setImage("Minimap","mapTile-blank");
        }else if(mapMgr->mts[xpos][ypos]->isEmpty()) {
            ic.setImage("Minimap","mapTile-blank");
        } else if(mapMgr->mts[xpos][ypos]->isObj()) {
            ic.setImage("MinimapNow","mapTileN-special");
        } else {
            std::stringstream tile;
            if(i==0) {
                tile << "mapTileN";
            } else if (mapMgr->mts[xpos][ypos]->isStart()) {
                tile << "mapTileS";
            } else if (mapMgr->mts[xpos][ypos]->isEnd()) {
                tile << "mapTileE";
            } else {
                tile << "mapTile";
            }
            
            appendTileEnding(tile, xpos, ypos, rotate);
            
            string name = tile.str();
            if(i==0) {
                ic.setImage("MinimapNow",name);
            } else if (mapMgr->mts[xpos][ypos]->isStart()) {
                ic.setImage("MinimapStart",name);
            } else if (mapMgr->mts[xpos][ypos]->isEnd()) {
                ic.setImage("MinimapEnd",name);
            } else {
                ic.setImage("Minimap",name);
            }
        }

        ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
        ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

        CEGUI::ComponentArea ca = CEGUI::ComponentArea();

        ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(mapx* 70 * wpixel,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
        ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(mapy* 70 * hpixel,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
        ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(70 * wpixel,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
        ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(70 * hpixel,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);

        ic.setComponentArea(ca);

        is.addImageryComponent(ic);

        i++;
    }

    lookFeel.addImagerySection(is);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(1);
    CEGUI::SectionSpecification ss = CEGUI::SectionSpecification("miniMap","enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    //CEGUI::WidgetLookManager::getSingleton().writeWidgetLookToStream("miniMap",std::cout);
    
    // Create the FrameWindow to return
    CEGUI::FrameWindow *minimap = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair"));
    minimap->setLookNFeel(lookFeel.getName());
    minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (229 - 10) * wpixel,0),CEGUI::UDim(1.0 - (229 - 9) * hpixel,0)));
    minimap->setSize(CEGUI::UVector2(CEGUI::UDim(210,0),CEGUI::UDim(210,0)));

    // minimap->setSize(CEGUI::UVector2(CEGUI::UDim(210 * wpixel,0),CEGUI::UDim(210 * hpixel,0)));

    return minimap;
}

void HUD::setStatus(std::string stat) {
    status->setText(stat);
}

void HUD::setLog(std::string wang) {
    if (wang == "") return;
    std::string newLog = log->getText().c_str();
    newLog += wang;
    newLog += '\n';
    log->setText(newLog);
    log->setCaratIndex(log->getText().size() - 1);
    log->ensureCaratIsVisible();
}

void HUD::setSpeedIndicator(double giraffe) {
	speedo0->setVisible(false);
	speedo1->setVisible(false);
	speedo2->setVisible(false);
	speedo3->setVisible(false);
	speedo4->setVisible(false);
	speedo5->setVisible(false);


    if (giraffe >= 120.0 && giraffe < 180.0) {
    	speedo1->setVisible(true);
    } else if (giraffe >= 180.0 && giraffe < 240.0) {
    	speedo2->setVisible(true);
    } else if (giraffe >= 240.0 && giraffe < 300.0) {
    	speedo3->setVisible(true);
    } else if (giraffe >= 300.0 && giraffe < 360.0) {
    	speedo4->setVisible(true);
    } else if (giraffe >= 360.0) {
    	speedo5->setVisible(true);
	} else {
		speedo0->setVisible(true);
	}
}

//void HUD::setShieldsbool HUD::hasControlScreenBeenShown() { return controlsBeenShown; }(float yeah) {
//    shields->setProgress(yeah);
//}

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

//void HUD::setSensorRate(float yeah) {
//    sensorRate->setProgress(yeah);
//}

void HUD::setWeaponRate(float what) {
    weaponRate->setProgress(what);
}

void HUD::setEngineRate(float alright) {
    engineRate->setProgress(alright);
}

void HUD::setWeaponCharge(float yeah) {
    weaponCharge->setProgress(yeah);
}

void HUD::setShieldCharge(float yeah) {
    shieldCharge->setProgress(yeah);
}

void HUD::setBossHealthbar(float ohshi) {
    bossHealthbar->setProgress(ohshi);
}

void HUD::setCountdown(std::string timer) {
	countdown->setText(timer);
}

void HUD::setTeamInfo(std::string nick1, std::string nick2, std::string nick3,
                      int killcount1, int killcount2, int killcount3,
                      bool repairing1, bool repairing2, bool repairing3 ) {
	teaminfo1->setText(nick1);
	teaminfo2->setText(nick2);
	teaminfo3->setText(nick3);
	std::stringstream k1;
	std::stringstream k2;
	std::stringstream k3;
	k1 << "Kills: " << killcount1;
	k2 << "Kills: " << killcount2;
	k3 << "Kills: " << killcount3;
	kills1->setText(k1.str());
	kills2->setText(k2.str());
	kills3->setText(k3.str());

    // TODO: Show a symbol indicating if players are repairing or not

    repairing1 ? repairAv1->setVisible(true) : repairAv1->setVisible(false);
	repairing2 ? repairAv2->setVisible(true) : repairAv2->setVisible(false);
	repairing3 ? repairAv3->setVisible(true) : repairAv3->setVisible(false);


}
	

void HUD::updateMiniMap() {
    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));

    float yaw = shipState->getOrientation()->y + 
        shipState->getMeshOrientation()->y;
    while(yaw < 0) yaw += 2 * PI;
    while(yaw >= 2 * PI) yaw -= 2 * PI;
    
    int rotate = (int)((yaw + PI / 4) / (PI / 2));

    if((x!=prevX)||(y!=prevY)||rotate!=prevRotate) {
        prevRotate = rotate;
        guiManager->getRootWindow()->removeChildWindow(minimap);
        minimap = buildMiniMap(rotate);
        guiManager->getRootWindow()->addChildWindow(minimap);
        guiManager->getRootWindow()->removeChildWindow(fullmap);
        fullmap = buildFullMap();
        guiManager->getRootWindow()->addChildWindow(fullmap);
    }
}

void HUD::toggleMap(bool tog)
{
    if(tog) {
        fullmap->setVisible(true);
    } else {
        fullmap->setVisible(false);
    }
}

void HUD::toggleControls(bool tog) {
    controls->setVisible(tog);
    if (tog) controlsBeenShown = true;
}

void HUD::toggleCrosshair(bool tog) {
    crosshair->setVisible(tog);
}

void HUD::switchStatus(int state) {
    statusIndicatorsStealth->setVisible(false);
    statusIndicatorsSwarms->setVisible(false);
    statusIndicatorsComrade->setVisible(false);
    statusIndicatorsBossHealth->setVisible(false);
    statusIndicatorsBlank->setVisible(false);
    bossHealthbar->setVisible(false);
    bossText->setVisible(false);
    
    // std::cout << state << std::endl;

    switch ( state ) {
        case 1:
            //statusIndicatorsStealth->setVisible(true);
            break;
        case 2:
            //statusIndicatorsSwarms->setVisible(true);
            break;
        case 3:
            statusIndicatorsComrade->setVisible(true);
            break;
        case 4:
            statusIndicatorsBossHealth->setVisible(true);
            bossHealthbar->setVisible(true);
            bossText->setVisible(true);
            break;
        case 5:
            statusIndicatorsBlank->setVisible(true);
            countdown->setVisible(true);
            break;
    }

}



    
