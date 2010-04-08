
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
{
    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    float noscale = 1.6 / ratio;
    float g = (1.0*winWidth)/1680.0;

    float wpixel = 1.0 / (float)winWidth * g;
    float hpixel = 1.0 / (float)winHeight * g;

    // Background Elements
    CEGUI::FrameWindow *xcrosshair  = guiManager->addStaticImage("XCrosshair",  0.5, 0.5,   0.05/ratio, 0.05,   "XCrosshair",   "XCross"  );
    // CEGUI::FrameWindow *overlay     = guiManager->addStaticImage("Overlay",     0.5, 0.5,   1.6/ratio,  1.0,    "Overlay",      "Overlay" );

	// TEST
	
	// CEGUI::FrameWindow *test     = guiManager->addStaticImage("ScoresFail",     0.5, 0.5,   1680 * wpixel,  1050 * hpixel,    "ScoresFail",      "Whole" );

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
    
    // Weapon & Shield Charges
    
    CEGUI::FrameWindow *charge        = guiManager->addStaticImagePix("Charge",         0.0,                 260 * hpixel, 48 * wpixel, 240 * hpixel, "Charge",        "hole"  );
    CEGUI::FrameWindow *charges       = guiManager->addStaticImagePix("Charges",      1.0 - 48*wpixel,  260 * hpixel, 48 * wpixel, 240 * hpixel, "Charges",        "Whole"  );


    // Left
    CEGUI::FrameWindow *left        = guiManager->addStaticImagePix("Left",         0.0,                 1.0 - 229 * hpixel, 566 * wpixel, 229 * hpixel, "Left",        "Main"  );

    // Right
 
    if(gameRole==PILOT)
    CEGUI::FrameWindow *rightPil    = guiManager->addStaticImagePix("RightPil",        1.0 - 229 * wpixel,  1.0 - 229 * hpixel, 229 * wpixel, 229 * hpixel, "RightPil",    "Main"  );
    if(gameRole==ENGINEER)
    CEGUI::FrameWindow *rightEng    = guiManager->addStaticImagePix("RightEng",        1.0 - 367 * wpixel,  1.0 - 230 * hpixel, 367 * wpixel, 230 * hpixel, "RightEng",    "Main"  );
    if(gameRole==NAVIGATOR)
    CEGUI::FrameWindow *rightNav    = guiManager->addStaticImagePix("RightNav",        1.0 - 229 * wpixel,  1.0 - 229 * hpixel, 229 * wpixel, 229 * hpixel, "RightNav",    "Main"  );

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

    // Speed Indicator
    if(gameRole==PILOT) { 
        indicator = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/IEditbox","indicator"));
        //indicator->setFont("DroidSansMono-big.font");
        guiManager->getRootWindow()->addChildWindow(indicator);
        indicator->setSize(CEGUI::UVector2(CEGUI::UDim(210 * wpixel,0),CEGUI::UDim(210 * hpixel,0)));
        indicator->setPosition(CEGUI::UVector2(CEGUI::UDim(1 - 191 * wpixel,0),CEGUI::UDim(1 - 219 * hpixel,0)));
    }
    
    // Status Indicator & Boss Health
    
    statusIndicatorsStealth    = guiManager->addStaticImage("StatusIndicators",     0.5, 1.0 - 23*hpixel,   206 * wpixel, 46 * hpixel, "StatusIndicators",        "Stealth"  );
    statusIndicatorsSwarms    = guiManager->addStaticImage("StatusIndicators2",     0.5, 1.0 - 23*hpixel,   206 * wpixel, 46 * hpixel, "StatusIndicators",        "Swarms"  );
    statusIndicatorsComrade    = guiManager->addStaticImage("StatusIndicators3",     0.5, 1.0 - 23*hpixel,   206 * wpixel, 46 * hpixel, "StatusIndicators",        "BlackComrade"  );
    statusIndicatorsBossHealth    = guiManager->addStaticImage("StatusIndicators4",     0.5, 1.0 - 23*hpixel,   206 * wpixel, 46 * hpixel, "StatusIndicators",        "BossHealthbar"  );    
    statusIndicatorsBlank = guiManager->addStaticImage("StatusIndicators5",     0.5, 1.0 - 23*hpixel,   206 * wpixel, 46 * hpixel, "StatusIndicators",        "Blank"  );    
    
    // statusIndicatorsStealth->setVisible(false);
    statusIndicatorsSwarms->setVisible(false);
    statusIndicatorsComrade->setVisible(false);
    statusIndicatorsBossHealth->setVisible(false);
    statusIndicatorsBlank->setVisible(false);
    
    // countdown timer
            
            
        countdown = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","countdown"));
//        countdown->setFont("DroidSansMono-big.font");
        guiManager->getRootWindow()->addChildWindow(countdown);
        countdown->setSize(CEGUI::UVector2(CEGUI::UDim(206 * wpixel,0),CEGUI::UDim(46 * hpixel,0)));
        countdown->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 103 * wpixel,0),CEGUI::UDim(1 - 46 * hpixel,0)));
        countdown->setVisible(false);
    
    // Progress Bars

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

    // Bars for power system stuff

    if (gameRole==ENGINEER) {
        shieldRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarShe","shieldRate"));
        //sensorRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarSen","sensorRate"));
        weaponRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarWep","weaponRate"));
        engineRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarEng","engineRate"));

        shieldRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        //sensorRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        weaponRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        engineRate->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));

        guiManager->getRootWindow()->addChildWindow(shieldRate);
        //guiManager->getRootWindow()->addChildWindow(sensorRate);
        guiManager->getRootWindow()->addChildWindow(weaponRate);
        guiManager->getRootWindow()->addChildWindow(engineRate);

        shieldRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 52 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68)* hpixel,0)));
        weaponRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 5 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68) * hpixel,0)));
        //sensorRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 98 ) * wpixel,0),CEGUI::UDim(1.0 - (228 - 70) * hpixel,0)));
        engineRate->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (367 - 98 ) * wpixel,0),CEGUI::UDim(1.0 - (230 - 68) * hpixel,0)));

        shieldRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));
        //sensorRate->setSize(CEGUI::UVector2(CEGUI::UDim(34 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));
        weaponRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));
        engineRate->setSize(CEGUI::UVector2(CEGUI::UDim(35 * wpixel,0),CEGUI::UDim(160 * hpixel,0)));

    
    }
    
    // Weapon and Shield Charge
    
        weaponCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarWep","weaponCharge"));
        shieldCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/VProgressBarShe","shieldCharge"));
      
        weaponCharge->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        shieldCharge->setProperty(CEGUI::String("VerticalProgress"),CEGUI::String("true"));
        
        guiManager->getRootWindow()->addChildWindow(weaponCharge);
        guiManager->getRootWindow()->addChildWindow(shieldCharge);
        
        weaponCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0 + 8*wpixel,0),CEGUI::UDim(300 * hpixel,0)));
        shieldCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 -  40*wpixel,0),CEGUI::UDim(300 * hpixel,0)));
        
        weaponCharge->setSize(CEGUI::UVector2(CEGUI::UDim(32 * wpixel,0),CEGUI::UDim(192 * hpixel,0)));
        shieldCharge->setSize(CEGUI::UVector2(CEGUI::UDim(32 * wpixel,0),CEGUI::UDim(192 * hpixel,0)));
        
        // Boss Health
        
        bossHealthbar = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBarComrade","Comrade"));
        guiManager->getRootWindow()->addChildWindow(bossHealthbar);
        bossHealthbar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5 - 104*wpixel,0),CEGUI::UDim(1.0 - 41 * hpixel,0)));
        bossHealthbar->setSize(CEGUI::UVector2(CEGUI::UDim(201 * wpixel,0),CEGUI::UDim(34 * hpixel,0)));
        bossHealthbar->setVisible(false);
    
    
    // Minimap

    if (gameRole==NAVIGATOR) {
        fullmap = buildFullMap();
        guiManager->getRootWindow()->addChildWindow(fullmap);
        fullmap->setVisible(false);

        // Create the minimap
        minimap = buildMiniMap();
        guiManager->getRootWindow()->addChildWindow(minimap);
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
            } else {
                std::stringstream tile;
                if((xpos==x)&&(ypos==y)) {
                    tile << "mapTileN";
                } else {
                    tile << "mapTile";
                }
                if(mapMgr->mts[xpos][ypos]->getAdjacent(1)!=0) {
                    tile << "-1";
                }
                if(mapMgr->mts[xpos][ypos]->getAdjacent(2)!=0) {
                    tile << "-2";
                }
                if(mapMgr->mts[xpos][ypos]->getAdjacent(3)!=0) {
                    tile << "-3";
                }
                if(mapMgr->mts[xpos][ypos]->getAdjacent(4)!=0) {
                    tile << "-4";
                }
                string name = tile.str();
                if((xpos==x)&&(ypos==y)) {
                    ic.setImage("Minimapnow",name);
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

CEGUI::FrameWindow* HUD::buildMiniMap() {

    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("miniMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    
    float g = (1.0*winWidth)/1680.0;
    float wpixel = 1.0 / (float)winWidth * g;
    float hpixel = 1.0 / (float)winHeight * g;


    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));
    prevX = x;
    prevY = y;
    int mapx = 1;
    int mapy = 1;

    int i=0;
    int xpos = x;
    int ypos = y;

    while(i<9) {
        if(i==1) {
            xpos = x-1; 
            ypos = y-1; 
            mapx = 0; 
            mapy = 0;
        }
        if(i==2) {
            xpos = x;
            ypos = y-1;
            mapx = 1;
            mapy = 0;
        }
        if(i==3) {
            xpos = x+1;
            ypos = y-1;
            mapx = 2;
            mapy = 0;
        }
        if(i==4) {
            xpos = x-1;
            ypos = y;
            mapx = 0;
            mapy = 1;
        }
        if(i==5) {
            xpos = x+1;
            ypos = y;
            mapx = 2;
            mapy = 1;
        }
        if(i==6) {
            xpos = x-1;
            ypos = y+1;
            mapx = 0;
            mapy = 2;
        }
        if(i==7) {
            xpos = x;
            ypos = y+1;
            mapx = 1;
            mapy = 2;
        }
        if(i==8) {
            xpos = x+1;
            ypos = y+1;
            mapx = 2;
            mapy = 2;
        }

        CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
        if(((xpos<0)||(ypos<0))||((xpos>Const::MAPSIZE)||(ypos>Const::MAPSIZE))) {
            ic.setImage("Minimap","mapTile-blank");
        }else if(mapMgr->mts[xpos][ypos]->isEmpty()) {
            ic.setImage("Minimap","mapTile-blank");
        } else {
            std::stringstream tile;
            if(i==0) {
                tile << "mapTileN";
            } else {
                tile << "mapTile";
            }
            if(mapMgr->mts[xpos][ypos]->getAdjacent(1)!=0) {
                tile << "-1";
            }
            if(mapMgr->mts[xpos][ypos]->getAdjacent(2)!=0) {
                tile << "-2";
            }
            if(mapMgr->mts[xpos][ypos]->getAdjacent(3)!=0) {
                tile << "-3";
            }
            if(mapMgr->mts[xpos][ypos]->getAdjacent(4)!=0) {
                tile << "-4";
            }
            string name = tile.str();
            if(i==0) {
                ic.setImage("Minimapnow",name);
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
    //minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(1.0 - (411 - 191) * wpixel,0),CEGUI::UDim(1.0 - (263 - 44) * hpixel,0)));
    minimap->setSize(CEGUI::UVector2(CEGUI::UDim(210 * wpixel,0),CEGUI::UDim(210 * hpixel,0)));

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

void HUD::setSpeedIndicator(std::string giraffe) {
    indicator->setText(giraffe);
}

//void HUD::setShields(float yeah) {
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

void HUD::updateMiniMap() {
    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));

    if((x!=prevX)||(y!=prevY)) {
        guiManager->getRootWindow()->removeChildWindow(minimap);
        minimap = buildMiniMap();
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

void HUD::switchStatus(int state) {
    statusIndicatorsStealth->setVisible(false);
    statusIndicatorsSwarms->setVisible(false);
    statusIndicatorsComrade->setVisible(false);
    statusIndicatorsBossHealth->setVisible(false);
    statusIndicatorsBlank->setVisible(false);
    bossHealthbar->setVisible(false);
    
    std::cout << state << std::endl;

    switch ( state ) {
        case 1:
            statusIndicatorsStealth->setVisible(true);
            break;
        case 2:
            statusIndicatorsSwarms->setVisible(true);
            break;
        case 3:
            statusIndicatorsComrade->setVisible(true);
            break;
        case 4:
            statusIndicatorsBossHealth->setVisible(true);
            bossHealthbar->setVisible(true);
            break;
        case 5:
            statusIndicatorsBlank->setVisible(true);
            countdown->setVisible(true);
            break;
    }

}



    
