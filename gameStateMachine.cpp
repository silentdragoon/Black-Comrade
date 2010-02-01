
#include "gameStateMachine.h"

#include <iostream>

GameStateMachine::GameStateMachine(MapManager *mapManager, ShipState *shipState)
	: gameState(GS_STELPH)
	, mapManager(mapManager)
	, shipState(shipState)
	, mIsNewState(true)
{}

GameStateMachine::GameStateMachine()
        : gameState(GS_STELPH)
        , previousState(GS_END)
        , mapManager(0)
        , shipState(0)
        , mIsNewState(true)
{}

void GameStateMachine::tick()
{
	GameState oldState = gameState;
	mIsNewState = false;

        if (mapManager == 0) {
            if (previousState != gameState) {
                mIsNewState = true;
                previousState = gameState;
            }
            return;
        }

        //std::cout << "Checking statemachine" << std::endl;
	// Waypoint events
	string *wp = mapManager->getWaypoint(shipState->position);
	if(wp != NULL) {
		if(*wp == "wp_attack") {
			switch(gameState) {
				case GS_STELPH:
					gameState = GS_ATTACK;
					break;
			}
		}
	}

	if(oldState != gameState) mIsNewState = true;
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
