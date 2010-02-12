#ifndef GUIMANAGER_H
#define GUIMANAGER_H

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
    MapManager *mapMgr;

    CEGUI::FrameWindow* buildMinimap();

public:
    GuiManager(MapManager *mapMgr);
    ~GuiManager();
    void setStatus(std::string stat);
    void moveMap();
};

#endif
