
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
{
    notificationQueue = std::vector<Notification>();
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
    notificationQueue = std::vector<Notification>();
}

void NotificationManager::tick()
{
	mIsNewNotification = false;

    if (mapManager == 0) {
        if (oldNotification != notification) {
            mIsNewNotification = true;
            oldNotification = notification;
        }
        return;
    }

	oldNotification = notification;
    //std::cout << "Checking statemachine" << std::endl;

    checkShipPosition();
    checkHealth();
    checkGameState();


}

void NotificationManager::checkGameState() {
    GameState gameState = stateMachine->currentGameState();
    switch(gameState) {
        case GS_STEALTH:
            notification = NT_CONTROLS;
            break;
        case GS_ATTACK:
            notification = NT_ATTACK;
            break;
    }
	if(oldNotification != notification) mIsNewNotification = true;        
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
    if (damageState->getHullHealth() <= 20) {
        notification = NT_HULL_CRITICAL;
    } else if (damageState->getEngineHealth() <= 20) {
        notification = NT_ENGINES_CRITICAL;
    } else if (damageState->getWeaponHealth() <= 20) {
        notification = NT_WEAPONS_CRITICAL;
    }
	if(oldNotification != notification) mIsNewNotification = true;
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
