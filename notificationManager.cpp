
#include "notificationManager.h"

#include <iostream>

NotificationManager::NotificationManager(GameStateMachine *stateMachine, MapManager *mapManager, ShipState *shipState, DamageState *damageState)
	: notification(NT_NONE)
    , oldNotification(NT_NONE)
    , stateMachine(stateMachine)
	, mapManager(mapManager)
	, shipState(shipState)
    , damageState(damageState)
	, mIsNewNotification(false)
    , tickcount(0)
{
    queue = std::vector<Notification>();
    recency = std::map<Notification,int>();

    int max = std::numeric_limits<int>::max();
    recency.insert(std::pair<Notification,int>(NT_CONTROLS,max));
    recency.insert(std::pair<Notification,int>(NT_ENGINES_CRITICAL,max));
    recency.insert(std::pair<Notification,int>(NT_WEAPONS_CRITICAL,max));
    recency.insert(std::pair<Notification,int>(NT_HULL_CRITICAL,max));
    recency.insert(std::pair<Notification,int>(NT_ATTACK,max));
}

NotificationManager::NotificationManager()
    : notification(NT_NONE)
    , oldNotification(NT_NONE)
    , stateMachine(0)
    , mapManager(0)
    , shipState(0)
    , damageState(0)
    , mIsNewNotification(false)
{
    queue = std::vector<Notification>();
}

void NotificationManager::updateRecencies() {
    if (tickcount < 1 / ConstManager::getFloat("tick_period")) {
        tickcount ++;
    } else {
        tickcount = 0;
        std::map<Notification,int>::const_iterator itr;
        for (itr = recency.begin() ; itr != recency.end() ; itr++) {
            if (itr->second == std::numeric_limits<int>::max()) break;
            recency[itr->first] = itr->second + 1;
            //std::cout << itr->second << std::endl;
        }
    }
}

void NotificationManager::tick()
{
	mIsNewNotification = false;
    updateRecencies();

    if (mapManager == 0) {
        if (oldNotification != notification) {
            mIsNewNotification = true;
            notify();
            if (notification != NT_NONE) oldNotification = notification;
        }
        return;
    }

    //std::cout << "Checking statemachine" << std::endl;

    //checkShipPosition();
    checkHealth();
    checkGameState();
    notify();
}

void NotificationManager::notify() {

    //if (oldNotification == notification) return;
    switch(notification) {
        case NT_CONTROLS:
            std::cout << "Here is how you use your controls..." << std::endl;
            break;
        case NT_ATTACK:
            std::cout << "You are under attack! Get ready for some intense s**t." << std::endl;
            break;
        case NT_ENGINES_CRITICAL:
            std::cout << "Engines are critical! Repair them quickly, or you'll be immobilised." << std::endl;
            break;
        case NT_WEAPONS_CRITICAL:
            std::cout << "Weapons are critical! Repair them quickly, or you'll be defenceless." << std::endl;
            break;
        case NT_HULL_CRITICAL:
            std::cout << "The hull is almost breached! Repair it quickly, or it's game over, men." << std::endl;
            break;
    }
    if (notification != NT_NONE) oldNotification = notification;
    recency[notification] = 0;
    notification = NT_NONE;
    //std::cout << recency[notification] << std::endl;
}

int NotificationManager::getTimeSince(Notification notification) {
    std::map<Notification, int>::const_iterator itr;
    itr = recency.find(notification);
    if (itr == recency.end()) {
        //Entity has not bee created
        return 0;
    } else {
        return itr->second;
    }
}

void NotificationManager::checkGameState() {
    GameState gameState = stateMachine->currentGameState();
    Notification newNotification;
    switch(gameState) {
        case GS_STEALTH:
            newNotification = NT_CONTROLS;
            break;
        case GS_ATTACK:
            newNotification = NT_ATTACK;
            break;
    }
	if(oldNotification != newNotification) {
        //std::cout << getTimeSince(newNotification) << std::endl;
        if (getTimeSince(newNotification) > 20) {
            mIsNewNotification = true;
            notification = newNotification;
        }
    }
}

void NotificationManager::checkShipPosition() {
    // Waypoint events
    //string *wp = mapManager->getWaypoint(shipState->getPosition());
    //if(wp != NULL) {
    //    if(*wp == "wp_attack") {
    //        switch(gameState) {
    //            case GS_STEALTH:
    //            gameState = GS_ATTACK;
    //            break;
    //        }
    //    }
    //}
}

void NotificationManager::checkHealth() {
    Notification newNotification;
    if (damageState->getHullHealth() <= 20) {
        newNotification = NT_HULL_CRITICAL;
    } else if (damageState->getEngineHealth() <= 20) {
        newNotification = NT_ENGINES_CRITICAL;
    } else if (damageState->getWeaponHealth() <= 20) {
        newNotification = NT_WEAPONS_CRITICAL;
    }
	if(oldNotification != newNotification) {
        if (getTimeSince(newNotification) > 20) {
            mIsNewNotification = true;
            notification = newNotification;
        }
    }
}
	
Notification NotificationManager::currentNotification() {
	return notification;
}

bool NotificationManager::isNewNotification() {
	return mIsNewNotification;
}

RakNet::RakString NotificationManager::GetName(void) const {return RakNet::RakString("NotificationManager");}

RM3SerializationResult NotificationManager::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(notification);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void NotificationManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(notification);
}
