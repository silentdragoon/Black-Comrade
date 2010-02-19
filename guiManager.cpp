#include "guiManager.h"

GuiManager::GuiManager(MapManager *mapMgr, ShipState *shipState) :
    shipState(shipState),
    prevX(10000),
    prevY(10000),
    mapMgr(mapMgr)
{
    CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");

    CEGUI::XMLParser *parser = CEGUI::System::getSingleton().getXMLParser();
    if(parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
        parser->setProperty("SchemaDefaultResourceGroup","schemas");
    }

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    // Set the scheme we want to use for skinning the gui and getting layouts and stuff
    CEGUI::SchemeManager::getSingleton().create("BlackComrade.scheme");
    CEGUI::FontManager::getSingleton().create("DejaVuSans-6.font");
    CEGUI::ImagesetManager::getSingleton().create("Minimap.imageset");
    CEGUI::ImagesetManager::getSingleton().create("Minimapnow.imageset");

    // Create the overall frame to add windows to
    guiRoot = guiMgr->createWindow("DefaultWindow","root");
    CEGUI::System::getSingleton().setGUISheet(guiRoot);

    // Add the crosshair to the default frame and centre it
    crossHair = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","crossHair"));
    guiRoot->addChildWindow(crossHair);
    crossHair->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f,0),CEGUI::UDim(0.5f,0)));

    // Add status output window to screen
    status = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/Editbox","status"));
    guiRoot->addChildWindow(status);
    status->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.05f,0)));
    status->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.95f,0)));

    // Health bars + Text for them
    shields = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","shields"));
    sensors = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","sensors"));
    weapons = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","weapons"));
    engine = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","engine"));
    hull = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","hull"));

    guiRoot->addChildWindow(shields);
    guiRoot->addChildWindow(sensors);
    guiRoot->addChildWindow(weapons);
    guiRoot->addChildWindow(engine);
    guiRoot->addChildWindow(hull);

    shields->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.0f,0)));
    sensors->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    weapons->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.06f,0)));
    engine->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.09f,0)));
    hull->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.12f,0)));

    shields->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    sensors->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    weapons->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    engine->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    hull->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));

    shieldText = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText","shieldtext"));
    guiRoot->addChildWindow(shieldText);
    shieldText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    shieldText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.0f,0)));
    shieldText->setText("1: Shields");

    sensorText = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText","sensortext"));
    guiRoot->addChildWindow(sensorText);
    sensorText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    sensorText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.03f,0)));
    sensorText->setText("2: Sensors");

    weaponText = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText","weapontext"));
    guiRoot->addChildWindow(weaponText);
    weaponText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    weaponText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.06f,0)));
    weaponText->setText("3: Weapons");

    engineText = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText","enginetext"));
    guiRoot->addChildWindow(engineText);
    engineText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    engineText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.09f,0)));
    engineText->setText("4: Engines");

    hullText = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText","hulltext"));
    guiRoot->addChildWindow(hullText);
    hullText->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.03f,0)));
    hullText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.12f,0)));
    hullText->setText("5: Hull");

    // Bars for power system stuff
    shieldRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","shieldRate"));
    sensorRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","sensorRate"));
    weaponRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","weaponRate"));
    engineRate = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","engineRate"));
    weaponCharge = static_cast<CEGUI::ProgressBar*>(guiMgr->createWindow("BlackComrade/ProgressBar","weaponCharge"));

    guiRoot->addChildWindow(shieldRate);
    guiRoot->addChildWindow(sensorRate);
    guiRoot->addChildWindow(weaponRate);
    guiRoot->addChildWindow(engineRate);
    guiRoot->addChildWindow(weaponCharge);

    shieldRate->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9f,0),CEGUI::UDim(0.0f,0)));
    weaponRate->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9f,0),CEGUI::UDim(0.03f,0)));
    sensorRate->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9f,0),CEGUI::UDim(0.06f,0)));
    engineRate->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9f,0),CEGUI::UDim(0.09f,0)));
    weaponCharge->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9f,0),CEGUI::UDim(0.12f,0)));

    shieldRate->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    sensorRate->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    weaponRate->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    engineRate->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));
    weaponCharge->setSize(CEGUI::UVector2(CEGUI::UDim(0.07f,0),CEGUI::UDim(0.03f,0)));

    // Create the full map 
    fullmap = buildFullMap();
    guiRoot->addChildWindow(fullmap);
    fullmap->setVisible(false);

    // Create the minimap
    minimap = buildMiniMap();
    guiRoot->addChildWindow(minimap);
}

GuiManager::~GuiManager(){}

CEGUI::FrameWindow* GuiManager::buildFullMap() {
    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("fullMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;

    // GET MAP PARTS HERE WITH A LOOOOOOP
    // -------------------------------------------------------------------

    for(int xpos=0;xpos<Const::MAPSIZE;xpos++) {
        for(int ypos=0;ypos<Const::MAPSIZE;ypos++) {
            CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
            if(mapMgr->mts[xpos][ypos]->isEmpty()) {
                ic.setImage("Minimap","mapTile-blank");
            } else {
                std::stringstream tile;
                tile << "mapTile";
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
                ic.setImage("Minimap",name);
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
    CEGUI::FrameWindow *fullmap = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","full"));
    fullmap->setLookNFeel(lookFeel.getName());
    fullmap->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5-30*0.015/ratio/2,0),CEGUI::UDim(0.5-30*0.015/2,0)));
    fullmap->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(1.0f,0)));

    return fullmap;
}

CEGUI::FrameWindow* GuiManager::buildMiniMap() {

    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("miniMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    int x =(int) floor(shipState->getPosition()->x/(double)Const::TILE_SIZE);
    int y =(int) floor(shipState->getPosition()->z/(double)Const::TILE_SIZE);
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

        ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(mapx*0.3/ratio,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
        ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(mapy*0.3,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
        ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(0.3/ratio,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
        ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(CEGUI::UDim(0.3,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);

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
    minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(0.75,0),CEGUI::UDim(0.75,0)));
    minimap->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f,0),CEGUI::UDim(0.2f,0)));

    return minimap;
}

void GuiManager::setStatus(std::string stat) {
    status->setText(stat);
}

void GuiManager::updateMiniMap() {
    int x =(int) floor(shipState->getPosition()->x/(double)Const::TILE_SIZE);
    int y =(int) floor(shipState->getPosition()->z/(double)Const::TILE_SIZE);

    if((x!=prevX)||(y!=prevY)) {
        guiRoot->removeChildWindow(minimap);
        minimap = buildMiniMap();
        guiRoot->addChildWindow(minimap);
    }
}

void GuiManager::toggleMap(bool tog)
{
    if(tog) {
        fullmap->setVisible(true);
    } else {
        fullmap->setVisible(false);
    }
}

void GuiManager::setShields(float yeah) {
    shields->setProgress(yeah);
}

void GuiManager::setSensors(float yeah) {
    sensors->setProgress(yeah);
}

void GuiManager::setWeapons(float yeah) {
    weapons->setProgress(yeah);
}

void GuiManager::setEngines(float yeah) {
    engine->setProgress(yeah);
}

void GuiManager::setHull(float yeah) {
    hull->setProgress(yeah);
}

void GuiManager::setShieldRate(float yeah) {
    shieldRate->setProgress(yeah);
}

void GuiManager::setSensorRate(float yeah) {
    sensorRate->setProgress(yeah);
}

void GuiManager::setWeaponRate(float yeah) {
    weaponRate->setProgress(yeah);
}

void GuiManager::setEngineRate(float yeah) {
    engineRate->setProgress(yeah);
}

void GuiManager::setWeaponCharge(float yeah) {
    weaponCharge->setProgress(yeah);
}

void GuiManager::setShieldText(string mess) {
    shieldText->setText(mess);
}

void GuiManager::setSensorText(string mess) {
    sensorText->setText(mess);
}

void GuiManager::setWeaponText(string mess) {
    weaponText->setText(mess);
}

void GuiManager::setEngineText(string mess) {
    engineText->setText(mess);
}

void GuiManager::setHullText(string mess) {
    hullText->setText(mess);
}
