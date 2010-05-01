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
#include "mapManager2.h"

enum KeyHint { KH_NONE,
               KH_TAB,
               KH_ESCAPE,
               KH_F1 };

enum HUDElement { HE_NONE,
                  HE_AVATARS,
                  HE_HEALTH_BARS,
                  HE_CHARGE_BARS,
                  HE_POWER_BARS,
                  HE_MINI_MAP,
                  HE_MINI_RADAR,
                  HE_MISSION_LOG };

class HUD {
private:

    float xCenter;
    float yCenter;
    float width;
    float height;

    float ratio;
    float wpixel,hpixel;

    int prevX;
    int prevY;
    int prevRotate;

    bool controlsBeenShown;
    bool largeMapBeenShown;
    bool largeRadarBeenShown;

    //CEGUI::ProgressBar *shields;
    CEGUI::ProgressBar *sensors;
    CEGUI::ProgressBar *weapons;
    CEGUI::ProgressBar *engine;
    CEGUI::ProgressBar *hull;

    CEGUI::ProgressBar *shieldRate;
    //CEGUI::ProgressBar *sensorRate;
    CEGUI::ProgressBar *weaponRate;
    CEGUI::ProgressBar *engineRate;
    CEGUI::ProgressBar *weaponCharge;
    CEGUI::ProgressBar *shieldCharge;
    
    CEGUI::ProgressBar *bossHealthbar;

    CEGUI::Editbox *status;
    CEGUI::MultiLineEditbox *log;
    CEGUI::Editbox *indicator;
    CEGUI::Editbox *countdown;
    CEGUI::Editbox *bossText;
    
    CEGUI::Editbox *teaminfo1;
    CEGUI::Editbox *teaminfo2;
    CEGUI::Editbox *teaminfo3;
    CEGUI::Editbox *kills1;
    CEGUI::Editbox *kills2;
    CEGUI::Editbox *kills3;

    CEGUI::FrameWindow *crosshair;
    CEGUI::FrameWindow *controls;

    CEGUI::FrameWindow *fullmap;
    CEGUI::FrameWindow *minimap;

    CEGUI::WindowManager *guiMgr;
    GuiManager *guiManager;
    MapManager *mapMgr;
    ShipState *shipState;

    CEGUI::FrameWindow* buildFullMap();
    CEGUI::FrameWindow* buildMiniMap(int rotate);
    
    CEGUI::FrameWindow *statusIndicatorsStealth;
    CEGUI::FrameWindow *statusIndicatorsSwarms;
    CEGUI::FrameWindow *statusIndicatorsComrade;
    CEGUI::FrameWindow *statusIndicatorsBossHealth;
    CEGUI::FrameWindow *statusIndicatorsBlank;

    CEGUI::FrameWindow *repairAv1;
    CEGUI::FrameWindow *repairAv2;
    CEGUI::FrameWindow *repairAv3;
    
    CEGUI::FrameWindow *speedo0;
    CEGUI::FrameWindow *speedo1;
    CEGUI::FrameWindow *speedo2;
    CEGUI::FrameWindow *speedo3;
    CEGUI::FrameWindow *speedo4;
    CEGUI::FrameWindow *speedo5;
    
    CEGUI::FrameWindow *hAvatars;
    CEGUI::FrameWindow *hCharge;
    CEGUI::FrameWindow *hHealthbars;
    CEGUI::FrameWindow *hMinimap;
    CEGUI::FrameWindow *hMissionlog;
    CEGUI::FrameWindow *hPowerbars;
    
    CEGUI::FrameWindow *keyHintWindow;
    CEGUI::FrameWindow *keyHintF1;
    CEGUI::FrameWindow *keyHintTab;
    CEGUI::FrameWindow *keyHintEsc;

    GameRole gameRole;
    
    void makeCommonHUD();

    void makePilotHUD();
    void makeNavigatorHUD();
    void makeEngineerHUD();

    void makeHighlightingHUD();

    void appendTileEnding(std::stringstream &ss, int xpos, int ypos, int rotate);

public:
    HUD(GuiManager *guiManager, ShipState *shipState, GameRole gameRole, MapManager *mapMgr);
    ~HUD();

    void showKeyHint(KeyHint key);
    void hideKeyHint();
    void highlightElement(HUDElement element);
    void stopHighlightingAllElements();
    void stopHighlightingElement(HUDElement element);

    void setStatus(std::string stat);
    void setLog(std::string wang);
    void setSpeedIndicator(double giraffe);

    //void setShields(float yeah);
    void setSensors(float yeah);
    void setWeapons(float yeah);
    void setEngines(float yeah);
    void setHull(float yeah);

    void setShieldRate(float yeah);
    //void setSensorRate(float yeah);
    void setWeaponRate(float yeah);
    void setEngineRate(float yeah);
    void setWeaponCharge(float yeah);
    void setShieldCharge(float yeah);
    
    void setTeamInfo(std::string nick1, std::string nick2, std::string nick3,
                     int killcount1, int killcount2, int killcount3,
                     bool repairing1, bool repairing2, bool repairing3);
   
    void setBossHealthbar(float yeah);
    void setCountdown(std::string timer);
    
    void switchStatus(int state);

    void toggleMap(bool tog);
    void updateMiniMap();

    void toggleControls(bool tog);

    void toggleCrosshair(bool tog);

    bool hasControlScreenBeenShown();
    bool hasLargeMapBeenShown();
    bool hasLargeRadarBeenShown();

};

#endif
