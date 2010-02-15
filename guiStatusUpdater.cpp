#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr,StateUpdate *stateUpdate, DamageState *damageState) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState)
{}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::tick() {
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << "Slack: " << slack << "ms";
    s = out.str();
    guiMgr->setStatus(s);

    float shieldHealth = (float)(damageState->getShieldHealth()/100.0);
    float sensorHealth = (float)(damageState->getSensorHealth()/100.0);       
    float weaponHealth = (float)(damageState->getWeaponHealth()/100.0);
    float engineHealth = (float)(damageState->getEngineHealth()/100.0);
    float hullHealth = (float)(damageState->getHullHealth()/100.0); 

    guiMgr->setShields(shieldHealth);
    guiMgr->setSensors(sensorHealth);
    guiMgr->setWeapons(weaponHealth);
    guiMgr->setEngines(engineHealth);
    guiMgr->setHull(hullHealth);

    if(shieldHealth<0.25f) guiMgr->setShieldText("1: CRITICAL");
    if(sensorHealth<0.25f) guiMgr->setSensorText("2: CRITICAL");
    if(weaponHealth<0.25f) guiMgr->setWeaponText("3: CRITICAL");
    if(engineHealth<0.25f) guiMgr->setEngineText("4: CRITICAL");
    if(hullHealth<0.25f) guiMgr->setHullText("5: CRITICAL");
}
