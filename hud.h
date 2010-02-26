#ifndef HUD_H
#define HUD_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include "guiManager.h"
#include "shipState.h"

class HUD {
private:

    float xCenter;
    float yCenter;
    float width;
    float height;

    CEGUI::ProgressBar *shields;
    CEGUI::ProgressBar *sensors;
    CEGUI::ProgressBar *weapons;
    CEGUI::ProgressBar *engine;
    CEGUI::ProgressBar *hull;

    CEGUI::WindowManager *guiMgr;
    GuiManager *guiManager;
    CEGUI::FrameWindow *minimap;
    ShipState *shipState;

public:
    HUD(GuiManager *guiManager, ShipState *shipState);
    ~HUD();

    void setShields(float yeah);
    void setSensors(float yeah);
    void setWeapons(float yeah);
    void setEngines(float yeah);
    void setHull(float yeah);
};

#endif
