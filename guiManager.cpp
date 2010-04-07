#include "guiManager.h"

CEGUI::Window *GuiManager::getRootWindow()
{
    return guiRoot;
}

GuiManager::GuiManager(SceneManager *sceneMgr)
    : sceneMgr(sceneMgr)
{
    CEGUI::OgreRenderer *ogreRenderer = &(CEGUI::OgreRenderer::bootstrapSystem());
    ogreRenderer->setRenderingEnabled(false);
    renderQueueListener = new CEGUIRQListener(ogreRenderer, Ogre::RENDER_QUEUE_OVERLAY, false);
    sceneMgr->addRenderQueueListener(renderQueueListener);

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
    CEGUI::FontManager::getSingleton().create("DroidSansMono.font");
    CEGUI::FontManager::getSingleton().create("DroidSansMono-big.font");
    CEGUI::ImagesetManager::getSingleton().create("Minimap.imageset");
    CEGUI::ImagesetManager::getSingleton().create("Minimapnow.imageset");
    // CEGUI::ImagesetManager::getSingleton().create("Crosshair.xml");

    // Create the overall frame to add windows to
    guiRoot = guiMgr->createWindow("DefaultWindow","root");

    CEGUI::System::getSingleton().setGUISheet(guiRoot);

    black = addStaticImage("Black",0.5, 0.5,1.0, 1.0,"Black","Black");
    black->setAlwaysOnTop(true);
    black->setAlpha(1.0f);
    black->hide();
}

GuiManager::~GuiManager(){}

void GuiManager::setOverlayAboveCEGUI(bool above) {
    renderQueueListener->setPostRenderQueue(!above);
}

CEGUI::FrameWindow *GuiManager::addStaticImage(const char *name, float xCenter, float yCenter,
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

bool GuiManager::fadeToBlack() {
    black->show();
    if (black->getAlpha() >= 1.0f) return true;

    black->setAlpha(black->getAlpha() + 0.05f);
    return false;
}

bool GuiManager::fadeFromBlack() {
    black->show();
    if (black->getAlpha() <= 0.0f) {
        black->hide();
        return true;
    }

    black->setAlpha(black->getAlpha() - 0.05f);
    return false;
}

CEGUI::FrameWindow *GuiManager::addStaticImagePix(const char *name, float xCenter, float yCenter,
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

CEGUI::FrameWindow * GuiManager::addStaticText(std::string name, std::string text, float xCenter, float yCenter,
                                               int size) {


    CEGUI::FrameWindow *textWindow = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow("BlackComrade/StaticText",name));
    textWindow->setPosition(CEGUI::UVector2(
        CEGUI::UDim(xCenter,-(textWindow->getFont()->getTextExtent( text ) + 12)/2),
        CEGUI::UDim(yCenter,-(textWindow->getFont()->getFontHeight() + 12 )/2)));

    textWindow->setSize(CEGUI::UVector2(
        CEGUI::UDim(0,textWindow->getFont()->getTextExtent( text ) + 12),
        CEGUI::UDim(0,textWindow->getFont()->getFontHeight() + 12 )));
    

    textWindow->setText(text);
    
    guiRoot->addChildWindow(textWindow);

    return textWindow;
}

void GuiManager::destroyAllWindows() {
    guiMgr->destroyAllWindows();
    guiRoot = guiMgr->createWindow("DefaultWindow","root");
    CEGUI::System::getSingleton().setGUISheet(guiRoot);
}

