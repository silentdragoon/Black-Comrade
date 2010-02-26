#ifndef HUD_H
#define HUD_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <sstream>
#include "guiManager.h"
#include "shipState.h"
#include "gameRole.h"

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

    CEGUI::ProgressBar *shieldRate;
    CEGUI::ProgressBar *sensorRate;
    CEGUI::ProgressBar *weaponRate;
    CEGUI::ProgressBar *engineRate;
    CEGUI::ProgressBar *weaponCharge;
    // CEGUI::ProgressBar *shieldCharge;

    CEGUI::Editbox *status;

    CEGUI::WindowManager *guiMgr;
    GuiManager *guiManager;
    ShipState *shipState;
    GameRole gameRole;

public:
    HUD(GuiManager *guiManager, ShipState *shipState, GameRole gameRole);
    ~HUD();

    void setStatus(std::string stat);

    void setShields(float yeah);
    void setSensors(float yeah);
    void setWeapons(float yeah);
    void setEngines(float yeah);
    void setHull(float yeah);

    void setShieldRate(float yeah);
    void setSensorRate(float yeah);
    void setWeaponRate(float yeah);
    void setEngineRate(float yeah);
    void setWeaponCharge(float yeah);
    // void setShieldCharge(float yeah);
};

#endif