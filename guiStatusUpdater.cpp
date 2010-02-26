#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr,StateUpdate *stateUpdate, DamageState *damageState, NavigatorControls *navigatorControls, GameRole gameRole, SystemManager *systemManager, HUD *hud, Flying *flying) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState),
    navigatorControls(navigatorControls),
    gameRole(gameRole),
    systemManager(systemManager),
    hud(hud),
    flying(flying)
{}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::tick() {

    // Display the slack in the game time
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << "Slack: " << slack << "ms";
    s = out.str();
    hud->setStatus(s);

    double speed = flying->getSpeed();
    cout << speed << endl;
    std::string t;
    std::stringstream outt;
    outt << " " << "m/s";
    t = outt.str();
    hud->setSpeedIndicator(t);

    // Deal with the damage of various systems
    float shieldHealth = (float)(damageState->getShieldHealth()/100.0);
    float sensorHealth = (float)(damageState->getSensorHealth()/100.0);
    float weaponHealth = (float)(damageState->getWeaponHealth()/100.0);
    float engineHealth = (float)(damageState->getEngineHealth()/100.0);
    float hullHealth = (float)(damageState->getHullHealth()/100.0); 

    hud->setShields(shieldHealth);
    hud->setSensors(sensorHealth);
    hud->setWeapons(weaponHealth);
    hud->setEngines(engineHealth);
    hud->setHull(hullHealth);

    // If player is navigator they can toggle the full screen map here
    if(gameRole==NAVIGATOR) {
        if(navigatorControls->isMap()) {
            guiMgr->toggleMap(true);
        } else {
            guiMgr->toggleMap(false);
        }
    }

    // Updates the images on the minimap
    // guiMgr->updateMiniMap();

    // Update the state of the power system bars
    // if(gameRole==ENGINEER) {
        float shieldRate = (float)(systemManager->getShieldRate());
        float weaponRate = (float)(systemManager->getWeaponRate());
        float sensorRate = (float)(systemManager->getSensorRate());
        float engineRate = (float)(systemManager->getEngineRate());
        float weaponCharge = (float)(systemManager->getWeaponCharge());
        
        hud->setShieldRate(shieldRate);
        hud->setSensorRate(sensorRate);
        hud->setWeaponRate(weaponRate);
        hud->setEngineRate(engineRate);
        hud->setWeaponCharge(weaponCharge/100.0);


    // }

}
