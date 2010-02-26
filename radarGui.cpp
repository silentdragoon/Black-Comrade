
#include "radarGui.h"

#define RADAR_TOP_FRAC 0.03
#define DOT_Y_OFFSET_FRAC 0.9
#define DOT_HEIGHT 0.1
#define RADAR_SIGHT_DIST 500
#define RADAR_ANGLE 1.570796

RadarGui::RadarGui(GuiManager *guiManager, ShipState *shipState,
    SwarmManager *swarmManager)
    : guiManager(guiManager)
    , shipState(shipState)
    , swarmManager(swarmManager)
    , xCenter(0.75)
    , yCenter(0.5)
    , width(0.5/1.33333333)
    , height(0.5)
    , uIndex(0)
    , radarWindow(NULL)
{
    //radarWindow = guiManager->addStaticImage("Radar",xCenter,yCenter,width,height,"Radar","background");
    
    //guiManager->getRootWindow()->addChildWindow(radarWindow);
}

void RadarGui::setDotPos(CEGUI::FrameWindow *dot, float x, float y)
{
    float globalX, globalY;

    // Handle offset of the dot    
    globalX = x;
    globalY = (1-y) - 0.2 * DOT_HEIGHT;

    // Handle size + pos of background
    globalX = globalX * width + xCenter - 0.5 * width;
    globalY = globalY * height + yCenter - 0.5 * height;
    
    dot->setPosition(CEGUI::UVector2( 
        CEGUI::UDim( globalX, 0 ), 
        CEGUI::UDim( globalY, 0 ) ) );
}

CEGUI::FrameWindow *RadarGui::createWindow(
    std::vector<std::pair<float,float> > *positions)
{
    char *name = "Radar";
    char *imageSet = "Radar";
    char *imageName = "background";

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
    
    std::stringstream frameName;
    frameName << "BlackComrade/";
    frameName << name;
    
    CEGUI::FrameWindow *window = static_cast<CEGUI::FrameWindow*>(guiMgr->createWindow(frameName.str().c_str(),name));
    window->setLookNFeel(lookFeel.getName());
    window->setPosition(CEGUI::UVector2(
        CEGUI::UDim(xCenter,0),CEGUI::UDim(yCenter,0)));
    
    window->setSize(CEGUI::UVector2(
        CEGUI::UDim(1,0),
        CEGUI::UDim(1,0)));

    return window;
}

CEGUI::FrameWindow *RadarGui::createEnemyDot()
{

    std::stringstream ss;
    
    ss << "Enemy";
    ss << uIndex++;

    return guiManager->addStaticImage("Enemy"
        ,0.0,0.0,DOT_HEIGHT,DOT_HEIGHT,"Radar","enemy");
}

void RadarGui::tick()
{
    std::vector<Swarm*> swarms = swarmManager->getAllSwarms();
    
    std::vector<std::pair<float,float> > positions;
    
    for(std::vector<Swarm*>::const_iterator it=swarms.begin();
        it!=swarms.end();++it) {
        
        Swarm *s = *it;
    
        Vector3 displacement = s->getAveragePosition() - 
            *shipState->getPosition();
            
        float dist = displacement.length() / RADAR_SIGHT_DIST;
        float angle = 0;//atan2(displacement.x,displacement.z);
        
        if(dist < 1 && abs(angle) < RADAR_ANGLE/2) {
            
            positions.push_back(std::pair<float,float>(dist, angle));
            
        }
    }

    if(radarWindow != NULL) {
        guiManager->getRootWindow()->removeChildWindow(radarWindow);
        guiMgr->destroyWindow(radarWindow);
    }

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    radarWindow = createWindow(&positions);
    guiManager->getRootWindow()->addChildWindow(radarWindow);
}

