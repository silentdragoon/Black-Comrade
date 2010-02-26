#include "guiManager.h"

CEGUI::Window *GuiManager::getRootWindow()
{
    return guiRoot;
}

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
    // CEGUI::ImagesetManager::getSingleton().create("Crosshair.xml");

    // Create the overall frame to add windows to
    guiRoot = guiMgr->createWindow("DefaultWindow","root");
    CEGUI::System::getSingleton().setGUISheet(guiRoot);

    // Add the crosshair to the default frame and centre it
    // crossHair = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","crossHair"));
    // guiRoot->addChildWindow(crossHair);
    // crossHair->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f,0),CEGUI::UDim(0.5f,0)));


    // Add status output window to screen
    // status = static_cast<CEGUI::Editbox*>(guiMgr->createWindow("BlackComrade/Editbox","status"));
    // guiRoot->addChildWindow(status);
    // status->setSize(CEGUI::UVector2(CEGUI::UDim(0.1f,0),CEGUI::UDim(0.05f,0)));
    // status->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f,0),CEGUI::UDim(0.95f,0)));




    // Create the full map 
    fullmap = buildFullMap();
    guiRoot->addChildWindow(fullmap);
    fullmap->setVisible(false);

    // Create the minimap
    minimap = buildMiniMap();
    guiRoot->addChildWindow(minimap);
    minimap->setVisible(false);             // TODO: remove this once it's in hud.cpp


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

CEGUI::FrameWindow *GuiManager::addStaticImage(char *name, float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName)
{
    guiMgr = CEGUI::WindowManager::getSingletonPtr();
    
    CEGUI::WidgetLookFeel lookFeel(name);
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery");
    
    CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
    ic.setImage(imageSet,imageName);
    
    ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
    ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

    CEGUI::ComponentArea ca = CEGUI::ComponentArea();
    
    ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(-(width/2),0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
    ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(-(height/2),0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
    ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(width,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
    ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(height,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);
        
    ic.setComponentArea(ca);

    is.addImageryComponent(ic);
    
    lookFeel.addImagerySection(is);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(1);
    CEGUI::SectionSpecification ss = 
        CEGUI::SectionSpecification(name,"enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    // TODO:: Almost certain some of these should be constants
    // Create the FrameWindow to return
    
    string frameName("BlackComrade/");
    frameName += string(name);
    
    CEGUI::FrameWindow *radarWindow = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow(frameName.c_str(),name));
    radarWindow->setLookNFeel(lookFeel.getName());
    radarWindow->setPosition(CEGUI::UVector2(
        CEGUI::UDim(xCenter,0),CEGUI::UDim(yCenter,0)));
    
    radarWindow->setSize(CEGUI::UVector2(
        CEGUI::UDim(1,0),
        CEGUI::UDim(1,0)));
    
    guiRoot->addChildWindow(radarWindow);
    
    return radarWindow;

}

CEGUI::FrameWindow *GuiManager::addStaticImagePix(char *name, float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName)
{
    guiMgr = CEGUI::WindowManager::getSingletonPtr();
    
    CEGUI::WidgetLookFeel lookFeel(name);
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery");
    
    CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
    ic.setImage(imageSet,imageName);
    
    ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
    ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

    CEGUI::ComponentArea ca = CEGUI::ComponentArea();
    
    ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(0,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
    ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(0,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
    ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(width,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
    ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(height,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);
        
    ic.setComponentArea(ca);

    is.addImageryComponent(ic);
    
    lookFeel.addImagerySection(is);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(1);
    CEGUI::SectionSpecification ss = 
        CEGUI::SectionSpecification(name,"enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    // TODO:: Almost certain some of these should be constants
    // Create the FrameWindow to return
    
    string frameName("BlackComrade/");
    frameName += string(name);
    
    CEGUI::FrameWindow *radarWindow = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow(frameName.c_str(),name));
    radarWindow->setLookNFeel(lookFeel.getName());
    radarWindow->setPosition(CEGUI::UVector2(
        CEGUI::UDim(xCenter,0),CEGUI::UDim(yCenter,0)));
    
    radarWindow->setSize(CEGUI::UVector2(
        CEGUI::UDim(1,0),
        CEGUI::UDim(1,0)));
    
    guiRoot->addChildWindow(radarWindow);
    
    return radarWindow;

}

void GuiManager::setStatus(std::string stat) {
    status->setText(stat);
}

void GuiManager::updateMiniMap() {
    int x =(int) floor(shipState->getPosition()->x/(double)ConstManager::getInt("map_tile_size"));
    int y =(int) floor(shipState->getPosition()->z/(double)ConstManager::getInt("map_tile_size"));

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
