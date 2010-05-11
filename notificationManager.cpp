
#include "notificationManager.h"

#include <iostream>

NotificationManager::NotificationManager(CollaborationInfo *collabInfo, GameStateMachine *stateMachine,
                                         MapManager *mapManager, ShipState *shipState,
                                         DamageState *damageState, SystemManager *systemManager,
                                         Tutorial *tutorial, Objective *objective)
    : collabInfo(collabInfo)
    , notification(new Notification(NT_NONE,"","",0))
    , lastNotification(new Notification(NT_NONE,"","",0))
    , stateMachine(stateMachine)
    , mapManager(mapManager)
    , shipState(shipState)
    , damageState(damageState)
    , systemManager(systemManager)
    , mIsNewNotification(false)
    , lastStateNotified(GS_END)
    , nextType(NT_NONE)
    , controlsDisplayed(false)
    , tickcount(2000)
    , tutorial(tutorial)
    , lastTutorialStateNotified(TS_END)
    , objective(objective)
{
    initializeRecencies();
}

NotificationManager::NotificationManager()
    : notification(new Notification(NT_NONE,"","",0))
    , lastNotification(new Notification(NT_NONE,"","",0))
    , nextType(NT_NONE)
    , collabInfo(0)
    , stateMachine(0)
    , mapManager(0)
    , shipState(0)
    , damageState(0)
    , systemManager(0)
    , tickcount(2000)
    , mIsNewNotification(false)
    , controlsDisplayed(false)
    , objective(0)
{
    initializeRecencies();
}

void NotificationManager::setTutorial(Tutorial *newTutorial) {
    tutorial = newTutorial;
}

void NotificationManager::initializeRecencies() {
    maxDelay = std::numeric_limits<int>::max();

    for (int i=0 ; i < NT_ENUM_END ; i++) {
        recency.insert(std::pair<NotificationType,int>((NotificationType)i,maxDelay));
    }

}

void NotificationManager::updateRecencies() {

    std::map<NotificationType,int>::const_iterator itr;
    for (itr = recency.begin() ; itr != recency.end() ; itr++) {
        if (itr->second != maxDelay) recency[itr->first] = itr->second + 1;
    }
}

void NotificationManager::tick()
{
    if (collabInfo == 0) {
        std::cerr << "NotificationManager has no collabinfo" << std::endl;
        exit(-1);
    }

    mIsNewNotification = false;

    if (mapManager == 0) {

        // Check for any local notifications
        if (tickcount < 1 / ConstManager::getFloat("tick_period")) {
            tickcount ++;
            return;
        } else {
            tickcount = 0;
            updateRecencies();
            checkTutorialState();
        }
        if (lastNotification->getType() != nextType) {
            // There's a new notification

            mIsNewNotification = true;

        }

        prepareNotification();

        if (nextType != NT_NONE) lastNotification = notification;

        // Print the notification to the terminal for now. Will be removed
        // when linked in with GUI/sound
        if (getCurrentNotification()->getType() != NT_NONE) {
            std::cout << getCurrentNotification()->getConsoleText();
        }
        return;
    }

    if (tickcount < 1 / ConstManager::getFloat("tick_period")) {
        tickcount ++;
    } else {
        tickcount = 0;
        updateRecencies();
        checkComments(); 
        checkShipPosition();
        checkCharges();
        checkHealth();
        checkTutorialState();
        checkCountdown();
        checkGameState();

        prepareNotification();

        // Print the notification to the terminal for now. Will be removed
        // when linked in with GUI/sound
        if (getCurrentNotification()->getType() != NT_NONE) {
            std::cout << getCurrentNotification()->getConsoleText();
        }
    }

}

std::string NotificationManager::pick(std::string sound, int num) {

    num = rand() % num + 1;
    std::stringstream temp;
    temp << "sound_" << sound << num;
    return temp.str();

}

void NotificationManager::prepareNotification() {
    std::stringstream consoleText;
    string soundNameConst = "";
    int soundLength = 0;
    bool local = false;
    srand ( time(NULL) );
    std::string gameRole = collabInfo->getGameRoleString();
    std::transform(gameRole.begin(), gameRole.end(), gameRole.begin(), ::tolower);
    switch(nextType) {

        // Game state notifications
        case NT_CONTROLS:
            consoleText << "Welcome aboard, " << collabInfo->getNick() << "."
                        << "\nWe know you're going to make a great " << gameRole << "."
                        << "\n\nWatch this space for further instructions." << std::endl;
            break;
        case NT_UNDER_ATTACK:
            soundNameConst = pick("IncomingSwarms",2);
            if (collabInfo->getGameRole() != ENGINEER)
                consoleText << "They're coming for us! You may want to ask your engineer to increase weapon power..." << std::endl;
            else
                consoleText << "They're coming for us! You may want to increase weapon power..." << std::endl;
            break;
        case NT_FLEE:
            soundNameConst = pick("ObjectiveDestroyed",2);
            if (collabInfo->getGameRole() == PILOT)
                consoleText << "You did it! Now you need to ask your navigator to find the exit, and escape." << std::endl;
            else if (collabInfo->getGameRole() == NAVIGATOR)
                consoleText << "You did it! Now use your large map to find the exit, and get the hell out of here." << std::endl;
            else if (collabInfo->getGameRole() == ENGINEER)
                consoleText << "You did it! You may want to increase engine power so we can escape quickly." << std::endl;
            break;
        case NT_OBJECTIVE_SEEK:
            soundNameConst = "sound_CommandOrdersBCDestruction";
            consoleText << "You should be ready for your mission now. Your first objective is to find the Black Comrade..." << std::endl;
            break;
        case NT_MISSION_COMPLETE:
           soundNameConst = "sound_MissionComplete";
            break;
        case NT_GAME_OVER:
            soundNameConst = "sound_GameOver";
            break;
        case NT_NEAR_BC:
            soundNameConst = "sound_NearingBC";
            break;
        case NT_SHIP_ON:
            soundNameConst = "sound_Startup";
            break;

        // Health notifications
        case NT_ENGINES_CRITICAL:
            soundNameConst = pick("EngineDamage",3);
            consoleText << "Engines are critical! Repair them quickly, or you'll be immobilised." << std::endl;
            break;
        case NT_WEAPONS_CRITICAL:
            soundNameConst = pick("WeaponDamage",5);
            consoleText << "Weapons are critical! Repair them quickly, or you'll be defenceless." << std::endl;
            break;
        case NT_HULL_CRITICAL:
            soundNameConst = pick("HullDamage",4);
            consoleText << "The hull is almost breached! Repair it quickly, or it's game over, men." << std::endl;
            break;
        case NT_SENSORS_CRITICAL:        // this one may need to be plugged in
            soundNameConst = pick("SensorDamage",2);
            consoleText << "Our sensors are almost destroyed! Repair them quickly, or we'll be flying blind!" << std::endl;

        // Random comments
        case NT_COMMENT_ONE:
            consoleText << "Keep up the good work, soldiers." << std::endl;
            break;
        case NT_COMMENT_TWO:
            consoleText << "We're all counting on you!" << std::endl;
            break;
        case NT_COMMENT_THREE:
            consoleText << "Let's show those commies who's boss." << std::endl;
            break;

        // Tutorial notifications
        case NT_TUT_START:
            consoleText << "Let's give you a quick reminder of how this ship works..." << std::endl;
            soundNameConst = "sound_common-01";
            break;
        case NT_TUT_MISSION_LOG:
            consoleText << "Here is the mission log..." << std::endl;
            soundNameConst = "sound_common-02";
            local = true;
            break;
        case NT_TUT_HEALTH_BARS:
            consoleText << "There are the health bars..." << std::endl;
            soundNameConst = "sound_common-03";
            local = true;
            break;
        case NT_TUT_CHARGE_BARS:
            consoleText << "There are the charge bars..." << std::endl;
            soundNameConst = "sound_common-05";
            local = true;
            break;
        case NT_TUT_AVATARS:
            consoleText << "There are the avatars..." << std::endl;
            soundNameConst = "sound_common-07";
            local = true;
            break;
        case NT_TUT_OPEN_CONSOLE:
            consoleText << "You can repair the ship using the console."
                        << "\n\nTry opening the console now by pressing the ESCAPE key." << std::endl;
            soundNameConst = "sound_tryopeningconsole";
            soundLength = 6;
            local = true;
            break;
        case NT_TUT_CLOSE_CONSOLE:
            consoleText << "Looks like you're familiar with the console now."
                        << "\n\nTry closing it by pressing the ESCAPE key again." << std::endl;
            soundNameConst = "sound_closelonger";
            soundLength = 6;
            local = true;
            break;
        case NT_TUT_PILOT_ROLE:
            consoleText << "You are the pilot...\n";
            soundNameConst = "sound_pilot-01";
            local = true;
            break;
        case NT_TUT_MOVE_SHIP:
            consoleText << "Why don't you try to move the ship now...\n";
            soundNameConst = "sound_tryflying";
            local = true;
            break;
        case NT_TUT_PILOT_END:
            consoleText << "Great. Keep flying, and hopefully your navigator will give you some directions soon.\n";
            soundNameConst = "sound_pilot-04";
            local = true;
            break;
        case NT_TUT_REPAIR_ENGINES:
            consoleText << "The ship's engines are damaged. Try and repair them using the console.\n";
            soundNameConst = "sound_fixengines";
            local = true;
            break;
        case NT_TUT_REPAIR_WEAPONS:
            consoleText << "The ship's weapons are damaged. Try and repair them using the console.\n";
            soundNameConst = "sound_fixweapons";
            local = true;
            break;
        case NT_TUT_REPAIR_SENSORS:
            consoleText << "The ship's sensors are damaged. Try and repair it using the console.\n";
            soundNameConst = "sound_fixsensors";
            local = true;
            break;
        case NT_TUT_NAVIGATOR_ROLE:
            consoleText << "You are the navigator...\n";
            soundNameConst = "sound_navigator-04";
            local = true;
            break;
        case NT_TUT_MINI_MAP:
            consoleText << "Here is your mini-map...\n";
            soundNameConst = "sound_navigator-05";
            local = true;
            break;
        case NT_TUT_SHOW_MAP:
            consoleText << "Try showing the large version using TAB...\n";
            soundNameConst = "sound_tryshowlargemap";
            local = true;
            break;
        case NT_TUT_CLOSE_MAP:
            consoleText << "You can use the large map to plan your route. Now close it again using TAB.\n";
            soundNameConst = "sound_closemap";
            local = true;
            break;
        case NT_TUT_NAVIGATOR_END:
            consoleText << "Great. Why don't you get familiar with the map, and give the pilot some instructions...\n";
            soundNameConst = "sound_navigator-06";
            local = true;
            break;
        case NT_TUT_ENGINEER_ROLE:
            consoleText << "You are the engineer...\n";
            soundNameConst = "sound_engineer-01";
            local = true;
            break;
        case NT_TUT_POWER_BARS:
            consoleText << "Here are the power bars...\n";
            soundNameConst = "sound_engineer-05";
            local = true;
            break;
        case NT_TUT_MINI_RADAR:
            consoleText << "Here is your mini-radar...\n";
            soundNameConst = "sound_engineer-06";
            local = true;
            break;
        case NT_TUT_SHOW_RADAR:
            consoleText << "Try showing the large version using TAB...\n";
            soundNameConst = "sound_tryshowlargeradar";
            local = true;
            break;
        case NT_TUT_CLOSE_RADAR:
            consoleText << "Some info about the large radar here, now close it again using TAB...\n";
            soundNameConst = "sound_closeradar";
            local = true;
            break;
        case NT_TUT_ENGINEER_END:
            consoleText << "Great. Why don't you get familiar with your radar, and give the pilot some instructions...\n";
            soundNameConst = "sound_engineer-07lol";
            local = true;
            break;
        case NT_TUT_WAITING:
            consoleText << "Great! We'll just wait while the other players finish getting to grips with the ship...\n";
            soundNameConst = "sound_wait";
            local = true;
            break;
        case NT_TUT_OPEN_DOORS:
            consoleText << "It's about time we got you moving. I'll open the blast doors for you now\n";
            soundNameConst = "sound_blastdoors";
            local = true;
            break;
        // Charge notifications
        case NT_WEAPON_CHARGE_STUCK:
            soundNameConst = "sound_WeaponChargeDepleted";
            consoleText << "Weapons are out of charge!\n";
            local = true;
            break;
        case NT_SHIELD_CHARGE_STUCK:
            soundNameConst = "sound_ShieldsDown";
            consoleText << "Shields are out of charge!\n";
            local = true;
            break;

        // Countdown notifications
        case NT_COUNTDOWN_5:
            soundNameConst = "sound_Count54321";
            break;
        case NT_COUNTDOWN_10:
            soundNameConst = "sound_Count10";
            break;
        case NT_COUNTDOWN_15:
            soundNameConst = "sound_Count15";
            break;
        case NT_COUNTDOWN_30:
            soundNameConst = "sound_Count30";
            break;
        case NT_COUNTDOWN_60:
            soundNameConst = "sound_Count60";
            break;

        // Other notifications
        case NT_DISOBEY:
            soundNameConst = pick("Disobey",2);
            break;

    }
    notification = new Notification(nextType,consoleText.str(),soundNameConst,soundLength);
    if (local) notification->makeLocal();
    if (nextType != NT_NONE) {
        lastNotification = notification;
        nextType = NT_NONE;
        recency[nextType] = 0;
    }
}

bool NotificationManager::isTimely(NotificationType type, int delaySinceMe, int delaySinceLast) {
    return (getTimeSince(type) > delaySinceMe &&
            getTimeSinceLast() > (delaySinceLast + lastNotification->getSoundLength()));
}

int NotificationManager::getTimeSince(NotificationType type) {
    std::map<NotificationType, int>::const_iterator itr;
    itr = recency.find(type);
    if (itr == recency.end()) {
        return 0;
    } else {
        return itr->second;
    }
}

int NotificationManager::getTimeSinceLast() {
    int last = std::numeric_limits<int>::max();
    std::map<NotificationType,int>::const_iterator itr;
    for (itr = recency.begin() ; itr != recency.end() ; itr++) {
        if (itr->second < last) last = itr->second;
    }
    return last;
}

void NotificationManager::checkGameState() {
    GameState gameState = stateMachine->currentGameState();
    if (lastStateNotified == gameState) return;
    NotificationType newNotification = lastNotification->getType();
    switch(gameState) {
        case GS_TUTORIAL:
            if (!controlsDisplayed)
                newNotification = NT_CONTROLS;
                controlsDisplayed = true;
            break;
        case GS_STEALTH:
            newNotification = NT_OBJECTIVE_SEEK;
            break;
        case GS_ATTACK:
            newNotification = NT_UNDER_ATTACK;
            break;
        case GS_FLEE:
            newNotification = NT_FLEE;
            break;
    }
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,0,1)) {
            mIsNewNotification = true;
            nextType = newNotification;
            lastStateNotified = gameState;
        }
    }
}

void NotificationManager::checkTutorialState() {
    TutorialState tutorialState = tutorial->getState();
    //if (lastTutorialStateNotified == tutorialState) return;
    NotificationType newNotification = lastNotification->getType();
    switch(tutorialState) {
        case TS_MISSION_LOG:
            newNotification = NT_TUT_MISSION_LOG;
            break;
        case TS_HEALTH_BARS:
            newNotification = NT_TUT_HEALTH_BARS;
            break;
        case TS_CHARGE_BARS:
            newNotification = NT_TUT_CHARGE_BARS;
            break;
        case TS_AVATARS:
            newNotification = NT_TUT_AVATARS;
            break;
        case TS_OPEN_CONSOLE:
            newNotification = NT_TUT_OPEN_CONSOLE;
            break;
        case TS_CLOSE_CONSOLE:
            newNotification = NT_TUT_CLOSE_CONSOLE;
            break;
        case TS_START:
            newNotification = NT_TUT_START;
            break;
        case TS_REPAIR_ENGINES:
            newNotification = NT_TUT_REPAIR_ENGINES;
            break;
        case TS_REPAIR_WEAPONS:
            newNotification = NT_TUT_REPAIR_WEAPONS;
            break;
        case TS_REPAIR_SENSORS:
            newNotification = NT_TUT_REPAIR_SENSORS;
            break;
        case TS_PILOT_ROLE:
            newNotification = NT_TUT_PILOT_ROLE;
            break;
        case TS_MOVE_SHIP:
            newNotification = NT_TUT_MOVE_SHIP;
            break;
        case TS_WAITING_FOR_OTHERS:
            newNotification = NT_TUT_WAITING;
            break;
        case TS_PILOT_END:
            newNotification = NT_TUT_PILOT_END;
            break;
        case TS_NAVIGATOR_ROLE:
            newNotification = NT_TUT_NAVIGATOR_ROLE;
            break;
        case TS_MINI_MAP:
            newNotification = NT_TUT_MINI_MAP;
            break;
        case TS_SHOW_MAP:
            newNotification = NT_TUT_SHOW_MAP;
            break;
        case TS_CLOSE_MAP:
            newNotification = NT_TUT_CLOSE_MAP;
            break;
        case TS_NAVIGATOR_END:
            newNotification = NT_TUT_NAVIGATOR_END;
            break;
        case TS_ENGINEER_ROLE:
            newNotification = NT_TUT_ENGINEER_ROLE;
            break;
        case TS_POWER_BARS:
            newNotification = NT_TUT_POWER_BARS;
            break;
        case TS_MINI_RADAR:
            newNotification = NT_TUT_MINI_RADAR;
            break;
        case TS_SHOW_RADAR:
            newNotification = NT_TUT_SHOW_RADAR;
            break;
        case TS_CLOSE_RADAR:
            newNotification = NT_TUT_CLOSE_RADAR;
            break;
        case TS_ENGINEER_END:
            newNotification = NT_TUT_ENGINEER_END;
            break;
        case TS_OPEN_DOORS:
            newNotification = NT_TUT_OPEN_DOORS;
            break;
        case TS_INDIVIDUAL:
            newNotification = NT_TUT_INDIVIDUAL;
            break;
    }

    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,1000,0)) {
            mIsNewNotification = true;
            nextType = newNotification;
            lastTutorialStateNotified = tutorialState;
        }
    }
}

void NotificationManager::checkCountdown() {
    NotificationType newNotification = lastNotification->getType();
    int countdown = objective->getEscapeTime();
    switch (countdown) {
        case 60:
            newNotification = NT_COUNTDOWN_60;
            break;
        case 30:
            newNotification = NT_COUNTDOWN_30;
            break;
        case 15:
            newNotification = NT_COUNTDOWN_15;
            break;
        case 10:
            newNotification = NT_COUNTDOWN_10;
            break;
        case 6:
            newNotification = NT_COUNTDOWN_5;
            break;
    }
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,30,0)) {
            mIsNewNotification = true;
            nextType = newNotification;
        }
    }
}

void NotificationManager::checkShipPosition() {
    //TODO - Base any notifications on map tiles?
}

void NotificationManager::checkComments() {
    srand ( time(NULL) );
    int irand = rand() % 3 + 1;

    if (tutorial->getState() != TS_END) return;

    NotificationType newNotification = lastNotification->getType();
    switch(irand) {
        case 1:
            newNotification = NT_COMMENT_ONE;
            break;
        case 2:
            newNotification = NT_COMMENT_TWO;
            break;
        case 3:
            newNotification = NT_COMMENT_THREE;
            break;
    }
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,180,30)) {
            mIsNewNotification = true;
            nextType = newNotification;
        }
    }
}

void NotificationManager::checkHealth() {
    NotificationType newNotification = lastNotification->getType();
    if (damageState->getHullHealth() <= 20) {
        newNotification = NT_HULL_CRITICAL;
    } else if (damageState->getEngineHealth() <= 20) {
        newNotification = NT_ENGINES_CRITICAL;
    } else if (damageState->getWeaponHealth() <= 20) {
        newNotification = NT_WEAPONS_CRITICAL;
    }
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,20,8)) {
            mIsNewNotification = true;
            nextType = newNotification;
        }
    }
}

void NotificationManager::checkCharges() {
    NotificationType newNotification = lastNotification->getType();
    GameState gameState = stateMachine->currentGameState();
    if (systemManager->areWeaponsStuck()
        && (gameState == GS_ATTACK || gameState == GS_BLACK_COMRADE))
        newNotification = NT_WEAPON_CHARGE_STUCK;
    else if (systemManager->areShieldsStuck()
        && (gameState == GS_ATTACK || gameState == GS_BLACK_COMRADE))
        newNotification = NT_SHIELD_CHARGE_STUCK;

    //std::cout << newNotification << "\n";
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,1000,1)) {
            mIsNewNotification = true;
            nextType = newNotification;
        }
    }
}

void NotificationManager::setCollaborationInfo(CollaborationInfo *mCollabInfo) {
    collabInfo = mCollabInfo;
}
    
Notification *NotificationManager::getCurrentNotification() {
    return notification;
}

bool NotificationManager::hasNewNotification() {
    return mIsNewNotification;
}

RakNet::RakString NotificationManager::GetName(void) const {return RakNet::RakString("NotificationManager");}

RM3SerializationResult NotificationManager::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(lastNotification->getType());
    if (lastNotification->isLocal()) return RM3SR_DO_NOT_SERIALIZE;
    return RM3SR_BROADCAST_IDENTICALLY;
}

void NotificationManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(nextType);
}
