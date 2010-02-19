
#include "notificationManager.h"

#include <iostream>

NotificationManager::NotificationManager(CollaborationInfo *collabInfo, GameStateMachine *stateMachine,
                                         MapManager *mapManager, ShipState *shipState,
                                         DamageState *damageState)
    : collabInfo(collabInfo)
    , notification(NT_NONE)
    , lastNotification(NT_NONE)
    , stateMachine(stateMachine)
    , mapManager(mapManager)
    , shipState(shipState)
    , damageState(damageState)
    , mIsNewNotification(false)
    , lastStateNotified(GS_END)
    , controlsDisplayed(false)
    , tickcount(2000)
{
    maxDelay = std::numeric_limits<int>::max();
    recency.insert(std::pair<Notification,int>(NT_CONTROLS,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_ENGINES_CRITICAL,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_WEAPONS_CRITICAL,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_HULL_CRITICAL,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_UNDER_ATTACK,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_COMMENT_ONE,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_COMMENT_TWO,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_COMMENT_THREE,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_OBJECTIVE_SEEK,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_OBJECTIVE_DESTROY,maxDelay));
    recency.insert(std::pair<Notification,int>(NT_OBJECTIVE_ESCAPE,maxDelay));
}

NotificationManager::NotificationManager()
    : notification(NT_NONE)
    , lastNotification(NT_NONE)
    , collabInfo(0)
    , stateMachine(0)
    , mapManager(0)
    , shipState(0)
    , damageState(0)
    , mIsNewNotification(false)
{}

void NotificationManager::updateRecencies() {

    std::map<Notification,int>::const_iterator itr;
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
        if (lastNotification != notification) {
            mIsNewNotification = true;
            notify();
            if (notification != NT_NONE) lastNotification = notification;
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
        checkHealth();
        checkGameState();

        notify();
    }
}

void NotificationManager::notify() {

    switch(notification) {
        case NT_CONTROLS:
            std::cout << "----------> Here's a reminder of your " << collabInfo->getGameRoleString()
                      << " controls, " << collabInfo->getNick() << std::endl;
            break;
        case NT_UNDER_ATTACK:
            std::cout << "----------> They're coming for us! You may want to increase shield and weapon power..." << std::endl;
            break;
        case NT_ENGINES_CRITICAL:
            std::cout << "----------> Engines are critical! Repair them quickly, or you'll be immobilised." << std::endl;
            break;
        case NT_WEAPONS_CRITICAL:
            std::cout << "----------> Weapons are critical! Repair them quickly, or you'll be defenceless." << std::endl;
            break;
        case NT_HULL_CRITICAL:
            std::cout << "----------> The hull is almost breached! Repair it quickly, or it's game over, men." << std::endl;
            break;
        case NT_COMMENT_ONE:
            std::cout << "----------> Random comment 1." << std::endl;
            break;
        case NT_COMMENT_TWO:
            std::cout << "----------> Random comment 2." << std::endl;
            break;
        case NT_COMMENT_THREE:
            std::cout << "----------> Random comment 3." << std::endl;
            break;
    }
    if (notification != NT_NONE) {
        lastNotification = notification;
        notification = NT_NONE;
        recency[notification] = 0;
    }
}

bool NotificationManager::isTimely(Notification notification, int delaySinceMe, int delaySinceLast) {
    return (getTimeSince(notification) > delaySinceMe && getTimeSinceLast() > delaySinceLast);
}

int NotificationManager::getTimeSince(Notification notification) {
    std::map<Notification, int>::const_iterator itr;
    itr = recency.find(notification);
    if (itr == recency.end()) {
        return 0;
    } else {
        return itr->second;
    }
}

int NotificationManager::getTimeSinceLast() {
    int last = std::numeric_limits<int>::max();
    std::map<Notification,int>::const_iterator itr;
    for (itr = recency.begin() ; itr != recency.end() ; itr++) {
        if (itr->second < last) last = itr->second;
    }
    return last;
}

void NotificationManager::checkGameState() {
    GameState gameState = stateMachine->currentGameState();
    if (lastStateNotified == gameState) return;
    Notification newNotification = lastNotification;
    switch(gameState) {
        case GS_STEALTH:
            if (!controlsDisplayed)
                newNotification = NT_CONTROLS;
                controlsDisplayed = true;
            break;
        case GS_ATTACK:
            newNotification = NT_UNDER_ATTACK;
            break;
    }
    if(lastNotification != newNotification) {
        if (isTimely(newNotification,0,8)) {
            mIsNewNotification = true;
            notification = newNotification;
            lastStateNotified = gameState;
        }
    }
}

void NotificationManager::checkShipPosition() {
    //TODO - Base any notifications on map tiles?
}

void NotificationManager::checkComments() {
    srand ( time(NULL) );
    int irand = rand() % 3 + 1;

    Notification newNotification = lastNotification;
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
    if(lastNotification != newNotification) {
        if (isTimely(newNotification,180,30)) {
            mIsNewNotification = true;
            notification = newNotification;
        }
    }
}

void NotificationManager::checkHealth() {
    Notification newNotification = lastNotification;
    if (damageState->getHullHealth() <= 20) {
        newNotification = NT_HULL_CRITICAL;
    } else if (damageState->getEngineHealth() <= 20) {
        newNotification = NT_ENGINES_CRITICAL;
    } else if (damageState->getWeaponHealth() <= 20) {
        newNotification = NT_WEAPONS_CRITICAL;
    }
    if(lastNotification != newNotification) {
        if (isTimely(newNotification,20,8)) {
            mIsNewNotification = true;
            notification = newNotification;
        }
    }
}

void NotificationManager::setCollaborationInfo(CollaborationInfo *mCollabInfo) {
    collabInfo = mCollabInfo;
}
	
Notification NotificationManager::getCurrentNotification() {
	return lastNotification;
}

bool NotificationManager::hasNewNotification() {
	return mIsNewNotification;
}

RakNet::RakString NotificationManager::GetName(void) const {return RakNet::RakString("NotificationManager");}

RM3SerializationResult NotificationManager::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(lastNotification);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void NotificationManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(notification);
}
