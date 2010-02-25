#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr,StateUpdate *stateUpdate, DamageState *damageState, NavigatorControls *navigatorControls, GameRole gameRole, SystemManager *systemManager) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState),
    navigatorControls(navigatorControls),
    gameRole(gameRole),
    systemManager(systemManager)
{}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::tick() {

    // Display the slack in the gmae time
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << "Slack: " << slack << "ms";
    s = out.str();
    guiMgr->setStatus(s);

    // Deal with the damage of various systems
    //float shieldHealth = (float)(damageState->getShieldHealth()/100.0);
    //float sensorHealth = (float)(damageState->getSensorHealth()/100.0);       
    float weaponHealth = (float)(damageState->getWeaponHealth()/100.0);
    //float engineHealth = (float)(damageState->getEngineHealth()/100.0);
    //float hullHealth = (float)(damageState->getHullHealth()/100.0); 

    //guiMgr->setShields(shieldHealth);
    //guiMgr->setSensors(sensorHealth);
    guiMgr->setWeapons(weaponHealth);
    //guiMgr->setEngines(engineHealth);
    //guiMgr->setHull(hullHealth);

    // If player is navigator they can toggle the full screen map here
    if(gameRole==NAVIGATOR) {
        if(navigatorControls->isMap()) {
            guiMgr->toggleMap(true);
        } else {
            guiMgr->toggleMap(false);
        }
    }

    // Updates the images on the minimap
    guiMgr->updateMiniMap();

    // Update the state of the power system bars
    if(gameRole==ENGINEER) {
        //float shieldRate = (float)(systemManager->getShieldRate());
        float weaponRate = (float)(systemManager->getWeaponRate());
        //float sensorRate = (float)(systemManager->getSensorRate());
        //float engineRate = (float)(systemManager->getEngineRate());
        //float weaponCharge = (float)(systemManager->getWeaponCharge());
        
        //guiMgr->setShieldRate(shieldRate);
        //guiMgr->setSensorRate(sensorRate);
        guiMgr->setWeaponRate(weaponRate);
        //guiMgr->setEngineRate(engineRate);
        //guiMgr->setWeaponCharge(weaponCharge/100.0);
    }
}
