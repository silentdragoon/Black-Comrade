
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapManager *mapManager, InputState *inputState,
                                   CollaborationInfo *pilotInfo, CollaborationInfo *engInfo,
                                   CollaborationInfo *navInfo, Tutorial *tutorial,
                                   ShipState *shipState, DamageState *damageState, Objective *objective)
    : gameState(GS_TUTORIAL)
    , mapManager(mapManager)
    , shipState(shipState)
    , damageState(damageState)
    , hullDamage(100.0)
    , waypointName(NULL)
    , isShipInSight(false)
    , mIsNewState(true)
    , objective(objective)
    , pilotInfo(pilotInfo)
    , engInfo(engInfo)
    , navInfo(navInfo)
    , inputState(inputState)
    , tutorial(tutorial)
{}

GameStateMachine::GameStateMachine()
    : gameState(GS_TUTORIAL)
    , oldState(GS_END)
    , mapManager(0)
    , shipState(0)
    , mIsNewState(true)
    , inputState(0)
    , tutorial(0)
{}

void GameStateMachine::tick()
{

    mIsNewState = false;

    checkInput();

    if (mapManager == 0) {
        if (oldState != gameState) {
            mIsNewState = true;
            oldState = gameState;
        }
        
        return;
    }

	oldState = gameState;
    //std::cout << "Checking statemachine" << std::endl;

    checkSwarms();
    checkObjective();
    checkHealth();
    checkWaypoints();
    checkTutorial();
    checkForQuit();

    if(oldState != gameState) mIsNewState = true;
}

void GameStateMachine::setInputState(InputState *newInputState) {
    inputState = newInputState;
}

void GameStateMachine::setWaypointName(std::string *mWaypointName) {
    waypointName = mWaypointName;
}

std::string* GameStateMachine::getWaypointName() { return waypointName; }

void GameStateMachine::setIsShipInSight(bool mIsShipInSight) {
    isShipInSight = mIsShipInSight;
}

bool GameStateMachine::getIsShipInSight() { return isShipInSight; }

double GameStateMachine::getHullDamage() { return hullDamage; }

void GameStateMachine::setHullDamage(double mHullDamage) {
    hullDamage = mHullDamage;
}

void GameStateMachine::checkWaypoints() {
    // Waypoint events
    std::vector<string*> strs;
    strs = mapManager->getWaypoints(shipState->getPosition());
    if(!strs.empty()) {
        for(std::vector<string*>::const_iterator it=strs.begin();it!=strs.end(); ++it) {
            string *wp = *it;
            if(*wp == "wp_attack") {
                switch(gameState) {
                    case GS_STEALTH:
                        gameState = GS_ATTACK;
                        break;
                }
            }
            if (*wp == "wp_end") {
                if (objective->getHealth() <=0.0)
                    gameState = GS_END;
            }
            if (*wp == "wp_tutorial_end") {
                switch(gameState) {
                    case GS_TUTORIAL:
                        tutorial->end();
                }
            }
        }
    }
}

void GameStateMachine::checkHealth() {
    if (damageState->getHullHealth() <= 0) {
        gameState = GS_GAME_OVER;
    }
}

void GameStateMachine::checkObjective() {
    if (objective->getEscapeTime() < 0 && gameState != GS_END) {
        gameState = GS_GAME_OVER;
    } else if (objective->getHealth() <= 0.0) {
        gameState = GS_FLEE;
    } else {
        MapTile *shipTile = mapManager->getMapTile(shipState->getPosition());
        MapTile *objectiveTile = mapManager->getMapTile(&mapManager->getObjectivePosition());
        if (shipTile == objectiveTile
            || objective->getHealth() < objective->getOriginalHealth())
            gameState = GS_BLACK_COMRADE;
    }
}

void GameStateMachine::checkSwarms() {
    switch(gameState) {
        case GS_STEALTH:
            if (isShipInSight) gameState = GS_ATTACK;
        break;
    }
}

void GameStateMachine::checkTutorial() {
    if (gameState == GS_TUTORIAL && tutorial->getState() == TS_END) {
        // The tutorial has been completed
        gameState = GS_STEALTH;
    }
}

void GameStateMachine::checkInput() {
    if (inputState->isKeyDown(OIS::KC_F10)) {
        pilotInfo->hasQuit = true;
        engInfo->hasQuit = true;
        navInfo->hasQuit = true;
    }
}

void GameStateMachine::setInfos(CollaborationInfo *nPilotInfo,
                                CollaborationInfo *nNavInfo,
                                CollaborationInfo *nEngInfo) {
    pilotInfo = nPilotInfo;
    navInfo = nNavInfo;
    engInfo = nEngInfo;
}

void GameStateMachine::checkForQuit() {
    bool playersQuit = (pilotInfo->hasQuit || engInfo->hasQuit || navInfo->hasQuit);
    if (playersQuit) {
        gameState = GS_GAME_OVER;
    }
}
	
GameState GameStateMachine::currentGameState()
{
	return gameState;
}

bool GameStateMachine::isNewState()
{
	return mIsNewState;
}

RakNet::RakString GameStateMachine::GetName(void) const {return RakNet::RakString("GameStateMachine");}

RM3SerializationResult GameStateMachine::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(gameState);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void GameStateMachine::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(gameState);
}
