#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include "mapManager.h"
#include "shipState.h"
#include "const.h"

class GuiManager {
private:

    CEGUI::WindowManager *guiMgr;
    CEGUI::Window *guiRoot;

    MapManager *mapMgr;
    ShipState *shipState;

public:
    GuiManager(MapManager *mapMgr, ShipState *shipState);
    ~GuiManager();

    CEGUI::FrameWindow *addStaticImage(const char *name, 
                                float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName);

    CEGUI::FrameWindow *addStaticImagePix(const char *name, float xCenter, float yCenter,
                                float width,   float height,
                                char *imageSet, char *imageName);
    void setStatus(std::string stat);
    CEGUI::Window *getRootWindow();
};

#endif
