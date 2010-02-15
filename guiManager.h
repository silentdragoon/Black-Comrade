#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include "mapManager.h"
#include "const.h"

class GuiManager {
private:
    CEGUI::WindowManager *guiMgr;
    CEGUI::Window *guiRoot;
    CEGUI::FrameWindow *crossHair;
    CEGUI::Editbox *status;
    CEGUI::FrameWindow *minimap;
    CEGUI::ProgressBar *shields;
    CEGUI::ProgressBar *sensors;
    CEGUI::ProgressBar *weapons;
    CEGUI::ProgressBar *engine;
    CEGUI::ProgressBar *hull;
    MapManager *mapMgr;

    CEGUI::FrameWindow* buildMinimap();

public:
    GuiManager(MapManager *mapMgr);
    ~GuiManager();
    void setStatus(std::string stat);
    void setShields(float yeah);
    void setSensors(float yeah);
    void setWeapons(float yeah);
    void setEngines(float yeah);
    void setHull(float yeah);
    void moveMap();
};

#endif
