#ifndef RADAR_GUI_H
#define RADAR_GUI_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include <deque>
#include "ITickable.h"
#include "guiManager.h"
#include "shipState.h"
#include "swarmManager.h"
#include "swarm.h"
#include "hud.h"

class RadarGui : public ITickable {
private:

    float xCenter;
    float yCenter;
    float width;
    float height;
    
    int uIndex;
    
    bool fullScreen;

    CEGUI::WindowManager *guiMgr;
    GuiManager *guiManager;
    HUD *hud;
    CEGUI::FrameWindow *minimap;
    CEGUI::ProgressBar *shields;
    CEGUI::FrameWindow *radarWindow;
    
    std::vector<CEGUI::FrameWindow*> enemyDots;
    
    ShipState *shipState;
    SwarmManager *swarmManager;
    
    void setDotPos(CEGUI::FrameWindow *dot, float x, float y);
    CEGUI::FrameWindow *createEnemyDot();
    
    CEGUI::FrameWindow *createWindow(
    std::vector<std::pair<float,float> > *positions);

public:
    RadarGui(GuiManager *guiManager, ShipState *shipState,
        SwarmManager *swarmManager, HUD *hud);
    //~RadarGui();
    
    void tick();
};

#endif
