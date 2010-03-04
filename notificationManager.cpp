
#include "notificationManager.h"

#include <iostream>

NotificationManager::NotificationManager(CollaborationInfo *collabInfo, GameStateMachine *stateMachine,
                                         MapManager *mapManager, ShipState *shipState,
                                         DamageState *damageState)
    : collabInfo(collabInfo)
    , notification(new Notification(NT_NONE,"",-1,0))
    , lastNotification(new Notification(NT_NONE,"",-1,0))
    , stateMachine(stateMachine)
    , mapManager(mapManager)
    , shipState(shipState)
    , damageState(damageState)
    , mIsNewNotification(false)
    , lastStateNotified(GS_END)
    , nextType(NT_NONE)
    , controlsDisplayed(false)
    , tickcount(2000)
{
    maxDelay = std::numeric_limits<int>::max();
    recency.insert(std::pair<NotificationType,int>(NT_CONTROLS,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_ENGINES_CRITICAL,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_WEAPONS_CRITICAL,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_HULL_CRITICAL,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_UNDER_ATTACK,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_COMMENT_ONE,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_COMMENT_TWO,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_COMMENT_THREE,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_OBJECTIVE_SEEK,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_OBJECTIVE_DESTROY,maxDelay));
    recency.insert(std::pair<NotificationType,int>(NT_OBJECTIVE_ESCAPE,maxDelay));
}

NotificationManager::NotificationManager()
    : notification(new Notification(NT_NONE,"",-1,0))
    , lastNotification(new Notification(NT_NONE,"",-1,0))
    , nextType(NT_NONE)
    , collabInfo(0)
    , stateMachine(0)
    , mapManager(0)
    , shipState(0)
    , damageState(0)
    , mIsNewNotification(false)
{}

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
        if (lastNotification->getType() != nextType) {
            mIsNewNotification = true;
            prepareNotification();
            // Print the notification to the terminal for now. Will be removed
            // when linked in with GUI/sound
            if (getCurrentNotification()->getType() != NT_NONE) {
                std::cout << getCurrentNotification()->getConsoleText();
            }
            if (nextType != NT_NONE) lastNotification = notification;
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

        prepareNotification();

        // Print the notification to the terminal for now. Will be removed
        // when linked in with GUI/sound
        if (getCurrentNotification()->getType() != NT_NONE) {
            std::cout << getCurrentNotification()->getConsoleText();
        }
    }

}

void NotificationManager::prepareNotification() {
    std::stringstream consoleText;
    int soundNameConst = -1;
    int soundLength = 0;
    switch(nextType) {
        case NT_CONTROLS:
            consoleText << "Here's a reminder of your " << collabInfo->getGameRoleString()
                      << " controls, " << collabInfo->getNick() << std::endl;
            break;
        case NT_UNDER_ATTACK:
            soundNameConst = 3;
            consoleText << "They're coming for us! You may want to increase shield and weapon power..." << std::endl;
            break;
        case NT_ENGINES_CRITICAL:
            consoleText << "Engines are critical! Repair them quickly, or you'll be immobilised." << std::endl;
            break;
        case NT_WEAPONS_CRITICAL:
            consoleText << "Weapons are critical! Repair them quickly, or you'll be defenceless." << std::endl;
            break;
        case NT_HULL_CRITICAL:
            consoleText << "The hull is almost breached! Repair it quickly, or it's game over, men." << std::endl;
            break;
        case NT_COMMENT_ONE:
            consoleText << "Random comment 1." << std::endl;
            break;
        case NT_COMMENT_TWO:
            consoleText << "Random comment 2." << std::endl;
            break;
        case NT_COMMENT_THREE:
            consoleText << "Random comment 3." << std::endl;
            break;
    }
    notification = new Notification(nextType,consoleText.str(),soundNameConst,soundLength);
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
        case GS_STEALTH:
            if (!controlsDisplayed)
                newNotification = NT_CONTROLS;
                controlsDisplayed = true;
            break;
        case GS_ATTACK:
            newNotification = NT_UNDER_ATTACK;
            break;
    }
    if(lastNotification->getType() != newNotification) {
        if (isTimely(newNotification,0,8)) {
            mIsNewNotification = true;
            nextType = newNotification;
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
    return RM3SR_BROADCAST_IDENTICALLY;
}

void NotificationManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(nextType);
}
