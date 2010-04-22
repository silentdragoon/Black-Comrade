#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState,
                                   GunnerControls *playerControls, GameRole gameRole,
                                   SystemManager *systemManager, HUD *hud, Flying *flying,
                                   NotificationManager *notificationMgr,
                                   GameStateMachine *gameStateMachine, Objective *objective, Console *console,
                                   CollaborationInfo *pilotInfo, CollaborationInfo *navInfo, CollaborationInfo *engInfo) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState),
    playerControls(playerControls),
    gameRole(gameRole),
    systemManager(systemManager),
    hud(hud),
    flying(flying),
    notificationMgr(notificationMgr),
    gameStateMachine(gameStateMachine),
    objective(objective),
    console(console),
    pilotInfo(pilotInfo),
    engInfo(engInfo),
    navInfo(navInfo)
{
    guiMgr->setOverlayAboveCEGUI(false);
}

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
        std::string t;
        std::stringstream outt;
        outt << int(speed) << " KM/H";
        t = outt.str();
        hud->setSpeedIndicator(t);
    }

    // Deal with the damage of various systems
    float sensorHealth = (float)(damageState->getSensorHealth()/100.0);
    float weaponHealth = (float)(damageState->getWeaponHealth()/100.0);
    float engineHealth = (float)(damageState->getEngineHealth()/100.0);
    float hullHealth = (float)(damageState->getHullHealth()/100.0); 

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

    // Toggle whether the controls are shown or not
    hud->toggleControls(playerControls->showControls());


    // If player is navigator they can toggle the full screen map here
    if(gameRole==NAVIGATOR) {
        if(((NavigatorControls*) playerControls)->isMap()) {
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
        float engineRate = (float)(systemManager->getEngineRate());

        
        hud->setShieldRate(shieldRate);
        hud->setWeaponRate(weaponRate);
        hud->setEngineRate(engineRate);
       }

    // TODO: Show player status / stats
    
    int kills1 = 0;
    int kills2 = 0;
    std::string rpr = "Repairing";
    std::string fgt = "";

    std::string nick1 = "";
    std::string nick2 = "";

    bool repairing1 = false;
    bool repairing2 = false;
    
    if (gameRole==PILOT) {
        repairing1 = navInfo->repairing;
        repairing2 = engInfo->repairing;
        kills1 = navInfo->getPlayerStats()->enemiesDestroyed;
        kills2 = engInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = navInfo->getNick();
        nick2 = engInfo->getNick();
	}
	if (gameRole==ENGINEER) {
        repairing1 = pilotInfo->repairing;
        repairing2 = navInfo->repairing;
	    kills1 = pilotInfo->getPlayerStats()->enemiesDestroyed;
	    kills2 = navInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = pilotInfo->getNick();
        nick2 = navInfo->getNick();
	}
	if (gameRole==NAVIGATOR) {
        repairing1 = pilotInfo->repairing;
        repairing2 = engInfo->repairing;
	    kills1 = pilotInfo->getPlayerStats()->enemiesDestroyed;
        kills2 = engInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = pilotInfo->getNick();
        nick2 = engInfo->getNick();
	}

    hud->setTeamInfo(nick1,nick2,kills1,kills2,repairing1,repairing2);

    // Hide crosshair if the console is open
    hud->toggleCrosshair(!console->getVisible());

    float weaponCharge = (float)(systemManager->getWeaponCharge());
    float shieldCharge = (float)(systemManager->getShieldCharge());
    hud->setWeaponCharge(weaponCharge/100.0);
    hud->setShieldCharge(shieldCharge/100.0);

    float bossHealth = objective->getHealthPercentage();
    hud->setBossHealthbar(bossHealth);

    int t = objective->getEscapeTime();
    std::stringstream count;
    count << " " << t << " seconds";
    hud->setCountdown(count.str());

    // Update transmission log

    if (notificationMgr->hasNewNotification()) {
        Notification *notification = notificationMgr->getCurrentNotification();
        if (notification->getType() != NT_NONE) {
            hud->setLog(notification->getConsoleText());
        }
    }
}
