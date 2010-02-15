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
    guiMgr->setShields((float)(damageState->getShieldHealth()/100.0));
    guiMgr->setSensors((float)(damageState->getSensorHealth()/100.0));
    guiMgr->setWeapons((float)(damageState->getWeaponHealth()/100.0));
    guiMgr->setEngines((float)(damageState->getEngineHealth()/100.0));
    guiMgr->setHull((float)(damageState->getHullHealth()/100.0));

    //guiMgr->moveMap();
}
