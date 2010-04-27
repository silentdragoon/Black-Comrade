#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState,
                                   GunnerControls *playerControls, GameRole gameRole,
                                   SystemManager *systemManager, HUD *hud, Flying *flying,
                                   NotificationManager *notificationMgr,
                                   GameStateMachine *gameStateMachine, Objective *objective, Console *console,
                                   CollaborationInfo *pilotInfo, CollaborationInfo *navInfo, CollaborationInfo *engInfo,
                                   Tutorial *tutorial) :
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
    navInfo(navInfo),
    tutorial(tutorial),
    flashLength(35),
    flashProgress(0),
    flashOn(false)
{
    guiMgr->setOverlayAboveCEGUI(false);
}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::updateFlash() {
    if (flashProgress == flashLength) {
        flashProgress = 0;
        flashOn = !flashOn;
    } else {
        flashProgress ++;
    }
}

void GuiStatusUpdater::checkTutorial() {

    switch(tutorial->getState()) {
        case TS_SHOW_CONTROLS:
            // Show the F1 key on screen?
            break;
        case TS_AVATARS:
            // Highlight avatars
            break;
        case TS_HEALTH_BARS:
            // Highlight health bars
            break;
        case TS_CHARGE_BARS:
            // Hightlight chargs bars
            break;
        case TS_POWER_BARS:
            // Highlight power bars
            break;
        case TS_MINI_MAP:
            // Highlight the mini map
            break;
        case TS_SHOW_MAP:
        case TS_SHOW_RADAR:
            // Show the tab key on screen?
            break;
        case TS_OPEN_CONSOLE:
        case TS_REPAIR_SYSTEMS:
            // Highlight the health bars?
        case TS_CLOSE_CONSOLE:
            // Show the escape key on screen?
            break;
    }
}

void GuiStatusUpdater::tick() {

    // Display the slack in the game time
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << slack << "ms";
    s = out.str();
    hud->setStatus(s);

    // Flash
    updateFlash();

    // Tutorial
    checkTutorial();

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
            case GS_BLACK_COMRADE:
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
        // Updates the images on the minimap
        hud->updateMiniMap();

        if(((NavigatorControls*) playerControls)->isMap()) {
            hud->toggleMap(true);
        } else {
            hud->toggleMap(false);
        }
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

    
    int kills1 = 0;
    int kills2 = 0;
    int kills3 = 0;
    std::string rpr = "Repairing";
    std::string fgt = "";

    std::string nick1 = "";
    std::string nick2 = "";
    std::string nick3 = "";

    bool repairing1 = false;
    bool repairing2 = false;
    bool repairing3 = false;
    
    if (gameRole==PILOT) {
        repairing1 = navInfo->repairing;
        repairing2 = engInfo->repairing;
        repairing3 = pilotInfo->repairing;
        kills1 = navInfo->getPlayerStats()->enemiesDestroyed;
        kills2 = engInfo->getPlayerStats()->enemiesDestroyed;
        kills3 = pilotInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = navInfo->getNick();
        nick2 = engInfo->getNick();
        nick3 = pilotInfo->getNick();
	}
	if (gameRole==ENGINEER) {
        repairing1 = pilotInfo->repairing;
        repairing2 = navInfo->repairing;
        repairing3 = engInfo->repairing;
	    kills1 = pilotInfo->getPlayerStats()->enemiesDestroyed;
	    kills2 = navInfo->getPlayerStats()->enemiesDestroyed;
	    kills3 = engInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = pilotInfo->getNick();
        nick2 = navInfo->getNick();
        nick3 = engInfo->getNick();
	}
	if (gameRole==NAVIGATOR) {
        repairing1 = pilotInfo->repairing;
        repairing2 = engInfo->repairing;
        repairing3 = navInfo->repairing;
	    kills1 = pilotInfo->getPlayerStats()->enemiesDestroyed;
        kills2 = engInfo->getPlayerStats()->enemiesDestroyed;
        kills3 = navInfo->getPlayerStats()->enemiesDestroyed;
        nick1 = pilotInfo->getNick();
        nick2 = engInfo->getNick();
        nick3 = navInfo->getNick();
	}

    hud->setTeamInfo(nick1,nick2,nick3,kills1,kills2,kills3,repairing1,repairing2,repairing3);

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
