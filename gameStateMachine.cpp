
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapManager *mapManager, ShipState *shipState, DamageState *damageState)
	: gameState(GS_STEALTH)
	, mapManager(mapManager)
	, shipState(shipState)
    , damageState(damageState)
    , hullDamage(100.0)
    , waypointName(NULL)
    , isShipInSight(false)
	, mIsNewState(true)
{}

GameStateMachine::GameStateMachine()
        : gameState(GS_STEALTH)
        , oldState(GS_STEALTH)
        , mapManager(0)
        , shipState(0)
        , mIsNewState(true)
{}

void GameStateMachine::tick()
{

	mIsNewState = false;

    if (mapManager == 0) {
        if (oldState != gameState) {
            mIsNewState = true;
            oldState = gameState;
        }
        return;
    }

	oldState = gameState;
    //std::cout << "Checking statemachine" << std::endl;

    checkWaypoints();
    checkSwarms();
    checkHealth();

	if(oldState != gameState) mIsNewState = true;
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
                // TODO: If the boss has not been destroyed,
                //       don't end the game...
                gameState = GS_END;
            }
        }
    }
}

void GameStateMachine::checkHealth() {
    if (damageState->getHullHealth() <= 0) {
        gameState = GS_GAME_OVER;
    }
}

void GameStateMachine::checkSwarms() {
    switch(gameState) {
        case GS_STEALTH:
            if (isShipInSight) gameState = GS_ATTACK;
        break;
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
