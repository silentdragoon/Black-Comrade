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
    CEGUI::FrameWindow *crossHair;
    CEGUI::Editbox *status;
    CEGUI::FrameWindow *fullmap;
    CEGUI::FrameWindow *minimap;
    CEGUI::ProgressBar *shields;
    CEGUI::ProgressBar *sensors;
    CEGUI::ProgressBar *weapons;
    CEGUI::ProgressBar *engine;
    CEGUI::ProgressBar *hull;
    CEGUI::FrameWindow *shieldText;
    CEGUI::FrameWindow *sensorText;
    CEGUI::FrameWindow *weaponText;
    CEGUI::FrameWindow *engineText;
    CEGUI::FrameWindow *hullText;

    MapManager *mapMgr;
    ShipState *shipState;

    CEGUI::FrameWindow* buildFullMap();

public:
    GuiManager(MapManager *mapMgr, ShipState *shipState);
    ~GuiManager();
    void setStatus(std::string stat);
    void setShields(float yeah);
    void setSensors(float yeah);
    void setWeapons(float yeah);
    void setEngines(float yeah);
    void setShieldText(string mess);
    void setSensorText(string mess);
    void setWeaponText(string mess);
    void setEngineText(string mess);
    void setHullText(string mess);
    void setHull(float yeah);
    void toggleMap(bool tog);
};

#endif
