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

    CEGUI::WindowManager *guiMgr;
    GuiManager *guiManager;
    CEGUI::FrameWindow *minimap;
    CEGUI::ProgressBar *shields;
    
    ShipState *shipState;

public:
    HUD(GuiManager *guiManager, ShipState *shipState);
    ~HUD();
};

#endif
