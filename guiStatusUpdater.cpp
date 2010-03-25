#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr,StateUpdate *stateUpdate, DamageState *damageState, NavigatorControls *navigatorControls, GameRole gameRole, SystemManager *systemManager, HUD *hud, Flying *flying, NotificationManager *notificationMgr, GameStateMachine *gameStateMachine, Objective *objective) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState),
    navigatorControls(navigatorControls),
    gameRole(gameRole),
    systemManager(systemManager),
    hud(hud),
    flying(flying),
    notificationMgr(notificationMgr),
    gameStateMachine(gameStateMachine),
    objective(objective)
{}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::tick() {

    // Display the slack in the game time
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << slack << "ms";
    s = out.str();
    hud->setStatus(s);

    // Ship speed
    if(gameRole==PILOT) {
        double speed = flying->getSpeed();
        speed = speed * 3.6 * 60;
        std::string t;
        std::stringstream outt;
        outt << int(speed) << " KPH";
        t = outt.str();
        hud->setSpeedIndicator(t);
    }

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
    
    // get game state and set HUD accordingly
    
    GameState gameState = gameStateMachine->currentGameState();
    if (gameStateMachine->isNewState()) {
        switch(gameState) {
            case GS_STEALTH:
                hud->switchStatus(1);
                break;
            case GS_ATTACK:
                hud->switchStatus(4);
                break;
            case GS_FLEE:
                hud->switchStatus(5);
                break;
         }
    }


    // If player is navigator they can toggle the full screen map here
    if(gameRole==NAVIGATOR) {
        if(navigatorControls->isMap()) {
            hud->toggleMap(true);
        } else {
            hud->toggleMap(false);
        }
        // Updates the images on the minimap
        hud->updateMiniMap();
   }

    // Update the state of the power system bars
    if(gameRole==ENGINEER) {
        float shieldRate = (float)(systemManager->getShieldRate());
        float weaponRate = (float)(systemManager->getWeaponRate());
        float sensorRate = (float)(systemManager->getSensorRate());
        float engineRate = (float)(systemManager->getEngineRate());

        
        hud->setShieldRate(shieldRate);
        hud->setSensorRate(sensorRate);
        hud->setWeaponRate(weaponRate);
        hud->setEngineRate(engineRate);
       }
       
    float weaponCharge = (float)(systemManager->getWeaponCharge());
    float shieldCharge = (float)(systemManager->getShieldCharge());
    hud->setWeaponCharge(weaponCharge/100.0);
    hud->setShieldCharge(shieldCharge/100.0);
    

    float bossHealth = objective->getHealth();
    cout << "bh: " << bossHealth << endl;
    hud->setBossHealthbar(bossHealth);

    // Update transmission log

    if (notificationMgr->hasNewNotification()) {
        Notification *notification = notificationMgr->getCurrentNotification();
        if (notification->getType() != NT_NONE) {
            hud->setLog(notification->getConsoleText());
        }
    }

}
