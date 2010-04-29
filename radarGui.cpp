
#include "radarGui.h"

#define RADAR_TOP_FRAC 0
#define DOT_Y_OFFSET_FRAC 0
#define DOT_HEIGHT 0.04
#define RADAR_SIGHT_DIST 1200
#define RADAR_ANGLE 1.04719755

RadarGui::RadarGui(GuiManager *guiManager, ShipState *shipState,
        SwarmManager *swarmManager, HUD *hud, bool fullScreen, char *name,
            EngineerControls *engineerControls, DamageState *damageState)
    : guiManager(guiManager)
    , shipState(shipState)
    , swarmManager(swarmManager)
    , xCenter(0.75)
    , yCenter(0.5)
    , width(0.5/1.33333333)
    , height(0.5)
    , uIndex(0)
    , radarWindow(NULL)
    , hud(hud)
    , fullScreen(fullScreen)
    , visible(true)
    , name(name)
    , engineerControls(engineerControls)
    , damageState(damageState)
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
    char *imageSet = "Radar";
    char *imageName;
    
    if(fullScreen)
        imageName = "backgroundFull";
    else
        imageName = "background";

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    CEGUI::WidgetLookFeel lookFeel(name);
    CEGUI::ImagerySection is = CEGUI::ImagerySection("enabled_imagery");

    // BACKGROUND

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

    // DOTS

    for(std::vector<std::pair<float,float> >::const_iterator it=
        positions->begin(); it!=positions->end();++it) {

        std::pair<float,float> pair = *it;

        float size;
        size = (fullScreen ? 0.5 : 1) * DOT_HEIGHT * height;

        // TODO: actully put stuff in the right place!

        float xPos, yPos;
        xPos = (fullScreen ? 0.5 : 1) * pair.first * sin(pair.second) - size;
        yPos = (fullScreen ? 0.5 : 1) * pair.first * cos(pair.second) - 0.5
            + 2 * size * DOT_Y_OFFSET_FRAC + (fullScreen ? 0.5 : 0);
        //cout << xPos << " - " << yPos << endl;
        ic = CEGUI::ImageryComponent();
        ic.setImage(imageSet,"enemy");

        ic.setVerticalFormatting(CEGUI::VF_STRETCHED);
        ic.setHorizontalFormatting(CEGUI::HF_STRETCHED);

        ca = CEGUI::ComponentArea();

        ca.d_left = CEGUI::Dimension(CEGUI::UnifiedDim(
            CEGUI::UDim(xPos*width,0),CEGUI::DT_X_POSITION),CEGUI::DT_X_POSITION);
        ca.d_top = CEGUI::Dimension(CEGUI::UnifiedDim(
            CEGUI::UDim(-yPos*height,0),CEGUI::DT_Y_POSITION),CEGUI::DT_Y_POSITION);
        ca.d_right_or_width = CEGUI::Dimension(CEGUI::UnifiedDim(
            CEGUI::UDim(size,0),CEGUI::DT_WIDTH),CEGUI::DT_WIDTH);
        ca.d_bottom_or_height = CEGUI::Dimension(CEGUI::UnifiedDim(
            CEGUI::UDim(size,0),CEGUI::DT_HEIGHT),CEGUI::DT_HEIGHT);

        ic.setComponentArea(ca);

        is.addImageryComponent(ic);

    }

    // Add to window..

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
    if(visible && radarWindow != NULL) {
        guiManager->getRootWindow()->removeChildWindow(radarWindow);
        guiMgr->destroyWindow(radarWindow);
    }

    if(fullScreen) {
        visible = engineerControls->isMap();
    }

    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float ratio = winWidth / (float)winHeight;
    float noscale = 1.6 / ratio;
    float g = (1.0*winWidth)/1680.0;

    float wpixel = 1.0 / (float)winWidth * g;
    float hpixel = 1.0 / (float)winHeight * g;

    if(fullScreen) {
    	width = 0.5 / ratio;
        height = 0.5;

        xCenter = 0.25;
        yCenter = 0.5;
    } else {
        width = 210 * wpixel;
        height = 210 * hpixel;

        xCenter = 1 - (191 + 27) * wpixel + width / 2;
        yCenter = 1 - (219 - 0) * hpixel + height / 2;
    }

    std::vector<Enemy*> enemies = (damageState->getSensorHealth() <= 0) ? std::vector<Enemy*>() : swarmManager->getAllLocalEnemies();

    std::vector<Enemy*> replicas = (damageState->getSensorHealth() <= 0) ? std::vector<Enemy*>() : swarmManager->getReplicatedEnemies();

    for(std::vector<Enemy*>::const_iterator it= replicas.begin(); it != replicas.end(); ++it) {
        enemies.push_back(*it);
    }

    std::vector<std::pair<float,float> > positions;

    for(std::vector<Enemy*>::const_iterator it=enemies.begin();
        it!=enemies.end();++it) {

        Enemy *s = *it;

        Vector3 displacement = *s->getPosition() -
            *shipState->getPosition();

        float dist = displacement.length() / RADAR_SIGHT_DIST;
        float angle = shipState->yaw - atan2(displacement.x,displacement.z);

        while(angle > PI) angle -= 2 * PI;
        while(angle < -PI) angle += 2 * PI;

        // If real close just draw dot
        //if(dist < 0.1) { dist = 0; angle = 0;}

        if(dist < 1 && (fullScreen || abs(angle) < RADAR_ANGLE/2)) {

            positions.push_back(std::pair<float,float>(dist, angle));

        }
    }

    guiMgr = CEGUI::WindowManager::getSingletonPtr();

    //positions.clear();
    //positions.push_back(std::pair<float,float>(0.4, -PI/4));

    if(visible) {
        radarWindow = createWindow(&positions);
        guiManager->getRootWindow()->addChildWindow(radarWindow);
    }
}

