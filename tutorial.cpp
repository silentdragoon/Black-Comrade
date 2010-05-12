#include "tutorial.h"
#include "radarGui.h"
#include "flying.h"

Tutorial::Tutorial(CollaborationInfo *tutee,
                   CollaborationInfo *tutee1, CollaborationInfo *tutee2, CollaborationInfo *tutee3,
                   GuiManager *guiMgr, MiniGameManager *miniGameMgr, DamageState *damageState,
                   SystemManager *systemMgr, ShipState *shipState, Door *door, InputState *inputState)
    : tutee(tutee)
    , tutee1(tutee1)
    , tutee2(tutee2)
    , tutee3(tutee3)
    , guiMgr(guiMgr)
    , inputState(inputState)
    , miniGameMgr(miniGameMgr)
    , damageState(damageState)
    , systemMgr(systemMgr)
    , shipState(shipState)
    , state(TS_PRE)
    , door(door)
    , flying(0)
    , pauseProgress(0)
{}

TutorialState Tutorial::getState() { return state; }

void Tutorial::setRadar(RadarGui *radar) {
    largeRadar = radar;
}

void Tutorial::setHUD(HUD *mHud) {
    hud = mHud;
}

void Tutorial::setFlying(Flying *mFlying) {
    flying = mFlying;
}

void Tutorial::tick() {
    if (state == TS_END) return;

    tickCommonTutorial();
    switch(tutee->getGameRole()) {
        case PILOT:
            tickPilotTutorial();
            break;
        case ENGINEER:
            tickEngineerTutorial();
            break;
        case NAVIGATOR:
            tickNavigatorTutorial();
            break;
    }
    checkForCompletion();
    if (state == TS_INDIVIDUAL)
        door->open();
}

void Tutorial::tickCommonTutorial() {
    if (inputState->isKeyDown(OIS::KC_END)) {
        // Skip the tutorial
        state = TS_WAITING_FOR_OTHERS;
    }

    switch(state) {
        case(TS_PRE) :
            changeWithPause(TS_START,3);
            break;
        case(TS_START) :
            changeWithPause(TS_MISSION_LOG,9);
            break;
        case(TS_MISSION_LOG) :
            // Highlight the mission log to the players
            changeWithPause(TS_HEALTH_BARS,8);
            break;
        case(TS_HEALTH_BARS) :
            changeWithPause(TS_CHARGE_BARS,13);
            break;
        case(TS_CHARGE_BARS) :
            changeWithPause(TS_AVATARS,12);
            break;
        case(TS_AVATARS) :
            changeWithPause(TS_OPEN_CONSOLE,7);
            break;
        case(TS_OPEN_CONSOLE) :
            // We want the player to open the console
            if (miniGameMgr->hasConsoleBeenOpened())
                changeWithPause(TS_REPAIR_SYSTEMS,0);
            break;
        case(TS_END_MINIGAME) :
             if(!miniGameMgr->isGameInProgress())
                 changeWithPause(TS_CLOSE_CONSOLE,0);
        case(TS_CLOSE_CONSOLE) :
            // We want the player to close the console
            if (miniGameMgr->hasConsoleBeenClosed())
               state = TS_WAITING_FOR_OTHERS;
            break;
        case(TS_OPEN_DOORS) :
            changeWithPause(TS_INDIVIDUAL,4);
    }
}

void Tutorial::tickPilotTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_ENGINES;
            break;
        case(TS_REPAIR_ENGINES) :
            if (damageState->getEngineHealth()>=95)
               changeWithPause(TS_END_MINIGAME,0);
            break;
        case(TS_INDIVIDUAL) :
            if (flying) flying->enableFlying();
            changeWithPause(TS_PILOT_ROLE,4);
            break;
        case(TS_PILOT_ROLE) :
            changeWithPause(TS_MOVE_SHIP,4);
            break;
        case(TS_MOVE_SHIP) :
            if (shipState->getSpeed() > 50)
                changeWithPause(TS_PILOT_END,4);
            break;
    }
}

void Tutorial::tickEngineerTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_WEAPONS;
            break;
        case(TS_REPAIR_WEAPONS) :
            if (damageState->getWeaponHealth()>=95)
               changeWithPause(TS_END_MINIGAME,0);
            break;
        case(TS_INDIVIDUAL) :
            changeWithPause(TS_ENGINEER_ROLE);
            break;
        case(TS_ENGINEER_ROLE) :
            changeWithPause(TS_POWER_BARS,4);
            break;
        case(TS_POWER_BARS) :
            changeWithPause(TS_CHANGE_POWERS,11);
            systemMgr->resetPowersBeenChanged();
            break;
        case(TS_MINI_RADAR) :
            changeWithPause(TS_SHOW_RADAR,11);
            break;
        case(TS_SHOW_RADAR) :
            if (largeRadar->hasBeenShown())
                changeWithPause(TS_ENGINEER_END,3);
            break;
        //case(TS_CLOSE_RADAR) :
        //    if (largeRadar->hasBeenClosed())
        //        changeWithPause(TS_CHANGE_POWERS,3);
        //    break;
        case(TS_CHANGE_POWERS) :
            if (systemMgr->havePowersBeenChanged())
                changeWithPause(TS_MINI_RADAR,2);
            break;
    }
}

void Tutorial::tickNavigatorTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_SENSORS;
            break;
        case(TS_REPAIR_SENSORS) :
            if (damageState->getSensorHealth()>=95)
               changeWithPause(TS_END_MINIGAME,0);
            break;
        case(TS_INDIVIDUAL) :
            changeWithPause(TS_NAVIGATOR_ROLE);
            break;
        case(TS_NAVIGATOR_ROLE) :
            changeWithPause(TS_MINI_MAP,4);
            break;
        case(TS_MINI_MAP) :
            changeWithPause(TS_SHOW_MAP,8);
            break;
        case(TS_SHOW_MAP) :
            if(hud->hasLargeMapBeenShown())
                changeWithPause(TS_CLOSE_MAP,3);
            break;
        case(TS_CLOSE_MAP) :
            changeWithPause(TS_NAVIGATOR_END,1);
            break;
    }
}

void Tutorial::changeWithPause(TutorialState newState, int pause) {
    if (pauseProgress > pause / ConstManager::getFloat("tick_period")) {
        state = newState;
        pauseProgress = 0;
    } else {
        pauseProgress ++;
    }
}

void Tutorial::checkForCompletion() {
    if (state == TS_WAITING_FOR_OTHERS) {
        tutee->hasCompletedTutorial = true;
        bool completed = (tutee1->hasCompletedTutorial &&
                          tutee2->hasCompletedTutorial &&
                          tutee3->hasCompletedTutorial);
        if (completed) changeWithPause(TS_OPEN_DOORS,4);
    }
}

void Tutorial::end() {
    state = TS_END;
}
