
#include "radarGui.h"

#define RADAR_TOP_FRAC 0.03
#define DOT_Y_OFFSET_FRAC 0.9
#define DOT_HEIGHT 0.1 

RadarGui::RadarGui(GuiManager *guiManager, ShipState *shipState)
    : guiManager(guiManager)
    , shipState(shipState)
    , xCenter(0.5)
    , yCenter(0.5)
    , width(0.5/1.33333333)
    , height(0.5)
{
    CEGUI::FrameWindow *radarWindow = guiManager->addStaticImage("Radar",xCenter,yCenter,width,height,"Radar","background");
    
    guiManager->getRootWindow()->addChildWindow(radarWindow);
    
    
    CEGUI::FrameWindow *enemyWindow = guiManager->addStaticImage("Enemy",0.0,0.0,DOT_HEIGHT,DOT_HEIGHT,"Radar","enemy");
    
    //enemyWindow->setRotation(CEGUI::Vector3(0,0,10));
    
    setDotPos(enemyWindow,1,0.5);
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

