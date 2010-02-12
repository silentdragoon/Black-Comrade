#include "guiManager.h"

GuiManager::GuiManager(MapManager *mapMgr) :
    mapMgr(mapMgr)
{
    CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    //CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    CEGUI::XMLParser *parser = CEGUI::System::getSingleton().getXMLParser();
    if(parser->isPropertyPresent("SchemaDefaultResourceGroup")) {
        parser->setProperty("SchemaDefaultResourceGroup","schemas");
    }

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    // Set the scheme we want to use for skinning the gui and getting layouts and stuff
    //CEGUI::SchemeManager::getSingleton().create("BlackComrade.scheme");
    CEGUI::SchemeManager::getSingleton().create("BlackComrade.scheme");
    CEGUI::FontManager::getSingleton().create("DejaVuSans-6.font");
    CEGUI::ImagesetManager::getSingleton().create("Minimap.imageset");

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
    status->setPosition(CEGUI::UVector2(CEGUI::UDim(0.05f,0),CEGUI::UDim(0.05f,0)));

    // Add minimap box to screen
    minimap = buildMinimap();
    guiRoot->addChildWindow(minimap);
}

GuiManager::~GuiManager(){}

CEGUI::FrameWindow* GuiManager::buildMinimap() {
    // CREATE MINIMAP
    CEGUI::WidgetLookFeel lookFeel("fullMiniMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    
    // GET MAP PARTS HERE WITH A LOOOOOOP
    // -------------------------------------------------------------------

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;

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

            // TODO: SCALING HERE NEEDS FIXING SO IT DOESNT TAKE UP ALL OF THE SCREEEEN
            CEGUI::ComponentArea ca = CEGUI::ComponentArea();
            /*
            ca.d_left = CEGUI::Dimension(CEGUI::AbsoluteDim(xpos*30),CEGUI::DT_X_POSITION);
            ca.d_top = CEGUI::Dimension(CEGUI::AbsoluteDim(ypos*30),CEGUI::DT_Y_POSITION);
            ca.d_right_or_width = CEGUI::Dimension(CEGUI::AbsoluteDim(30),CEGUI::DT_WIDTH);
            ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::AbsoluteDim(30),CEGUI::DT_HEIGHT);
            */

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
    CEGUI::SectionSpecification ss = CEGUI::SectionSpecification("fullMiniMap","enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    
    // Create the FrameWindow to return
    CEGUI::FrameWindow *minimap = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","mini"));
    minimap->setLookNFeel(lookFeel.getName());
    minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5-30*0.015/ratio/2,0),CEGUI::UDim(0.5-30*0.015/2,0)));
    //minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(0,0),CEGUI::UDim(0,0)));
    minimap->setSize(CEGUI::UVector2(CEGUI::UDim(1.0f,0),CEGUI::UDim(1.0f,0)));

    return minimap;
}

void GuiManager::setStatus(std::string stat) {
    status->setText(stat);
}

void GuiManager::moveMap()
{
    minimap->setPosition(minimap->getPosition() + CEGUI::UVector2(CEGUI::UDim(-0.0001f,0),CEGUI::UDim(-0.0001f,0)));
}
