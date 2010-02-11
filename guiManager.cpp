#include "guiManager.h"

GuiManager::GuiManager()
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
    CEGUI::WidgetLookFeel lookFeel("fullMiniMap");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery"); 
    lookFeel.addImagerySection(is);

    CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
    ic.setImage("Minimap","mapTile-1-2-3-4");

    //CEGUI::ImageryComponent ic2 = CEGUI::ImageryComponent();
    //ic2.setImage("Minimap","mapTile-1-2-3");

    is.addImageryComponent(ic);
    //is.addImageryComponent(ic2);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");
    lookFeel.addStateSpecification(si);

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(10000000);
    CEGUI::SectionSpecification ss = CEGUI::SectionSpecification("fullMiniMap","enabled_imagery","true");
    
    CEGUI::ComponentArea ca = CEGUI::ComponentArea();
    ca.d_left = CEGUI::Dimension(CEGUI::AbsoluteDim(0),CEGUI::DT_X_POSITION);
    ca.d_top = CEGUI::Dimension(CEGUI::AbsoluteDim(0),CEGUI::DT_Y_POSITION);
    ca.d_right_or_width = CEGUI::Dimension(CEGUI::AbsoluteDim(100),CEGUI::DT_WIDTH);
    ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::AbsoluteDim(100),CEGUI::DT_HEIGHT);
    
    ic.setImagePropertySource(ca.getAreaPropertySource());
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);
    CEGUI::WidgetLookManager::getSingleton().writeWidgetLookToStream("fullMiniMap",std::cout);
    
    minimap = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/CrossHair","efegfe"));
    minimap->setLookNFeel(lookFeel.getName());
    guiRoot->addChildWindow(minimap);
    minimap->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f,0),CEGUI::UDim(0.5f,0)));
    minimap->setSize(CEGUI::UVector2(CEGUI::UDim(0.2f,0),CEGUI::UDim(0.2f,0)));
}

GuiManager::~GuiManager(){}

void GuiManager::setStatus(std::string stat) {
    status->setText(stat);
}

void GuiManager::moveMap()
{

    minimap->setPosition(minimap->getPosition() + CEGUI::UVector2(CEGUI::UDim(-0.0001f,0),CEGUI::UDim(-0.0001f,0)));
}
