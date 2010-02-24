
#include "radarGui.h"

#define RADAR_TOP_FRAC 0.03
#define DOT_Y_OFFSET_FRAC 0.9

RadarGui::RadarGui(GuiManager *guiManager, ShipState *shipState)
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

    guiMgr = CEGUI::WindowManager::getSingletonPtr();
    
    CEGUI::WidgetLookFeel lookFeel("Radar");
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery");
    
    CEGUI::ImageryComponent ic = CEGUI::ImageryComponent();
    ic.setImage("Radar","background");
    
    ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
    ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

    CEGUI::ComponentArea ca = CEGUI::ComponentArea();
    
    ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(-0.5,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
    ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(-0.5,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
    ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(1,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
    ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(
        CEGUI::UDim(1,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);
        
    ic.setComponentArea(ca);

    is.addImageryComponent(ic);
    
    lookFeel.addImagerySection(is);

    CEGUI::StateImagery si = CEGUI::StateImagery("Enabled");

    CEGUI::LayerSpecification ls = CEGUI::LayerSpecification(1);
    CEGUI::SectionSpecification ss = 
        CEGUI::SectionSpecification("Radar","enabled_imagery","");
    
    ls.addSectionSpecification(ss);
    si.addLayer(ls);
    
    lookFeel.addStateSpecification(si);

    CEGUI::WidgetLookManager::getSingleton().addWidgetLook(lookFeel);

    CEGUI::FrameWindow *radarWindow = 
        static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow(
        "BlackComrade/Radar","Radar"));
    radarWindow->setLookNFeel(lookFeel.getName());
    radarWindow->setPosition(CEGUI::UVector2(
        CEGUI::UDim(xCenter,0),CEGUI::UDim(yCenter,0)));
    
    width = 1/ratio;
    height = 1;
    
    radarWindow->setSize(CEGUI::UVector2(
        CEGUI::UDim(width,0),
        CEGUI::UDim(height,0)));
    
    guiManager->getRootWindow()->addChildWindow(radarWindow);
    
    //CEGUI::FrameWindow *enemyWindow = guiManager->addStaticImage("Enemy",0.5,0.5,0.05/ratio,0.05,"Radar","enemy");
    
    CEGUI::FrameWindow *enemyWindow = guiManager->addStaticImage("Enemy",0.5,0.5,1,1,"Radar","enemy");
    
    //enemyWindow->setRotation(CEGUI::Vector3(0,0,10));
    
    //setDotPos(enemyWindow,0.5,0);
}

void RadarGui::setDotPos(CEGUI::FrameWindow *dot, float x, float y)
{
    float globalX, globalY;
    
    globalX = x * width + xCenter - width/2;
    
    globalY = y * height + yCenter - width/2;
    globalY *= DOT_Y_OFFSET_FRAC;
    

    dot->setPosition(CEGUI::UVector2( 
        CEGUI::UDim( x, 0 ), 
        CEGUI::UDim( 1-RADAR_TOP_FRAC, 0 ) ) );
}
