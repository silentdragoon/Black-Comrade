
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapManager *mapManager, ShipState *shipState, DamageState *damageState)
	: gameState(GS_STEALTH)
	, mapManager(mapManager)
	, shipState(shipState)
        , damageState(damageState)
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
        checkHealth();

	if(oldState != gameState) mIsNewState = true;
}

void GameStateMachine::checkWaypoints() {
    // Waypoint events
    string *wp = mapManager->getWaypoint(shipState->getPosition());
    if(wp != NULL) {
        if(*wp == "wp_attack") {
            switch(gameState) {
                case GS_STEALTH:
                gameState = GS_ATTACK;
                break;
            }
        }
    }
}

void GameStateMachine::checkHealth() {
    if (damageState->getHullHealth() <= 0) {
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
