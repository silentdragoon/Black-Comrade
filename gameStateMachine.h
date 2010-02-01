
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "ITickable.h"
#include "mapManager.h"
#include "shipState.h"
#include "replicaObject.h"

enum GameState { GS_STELPH, GS_ATTACK, GS_FLEE, GS_GAME_OVER, GS_END };

class GameStateMachine : public ITickable, public ReplicaObject
{
private:
	GameState gameState;
        GameState previousState;
	MapManager *mapManager;
	ShipState *shipState;
	bool mIsNewState;
	
public:
        GameStateMachine();
	GameStateMachine(MapManager *mapManager, ShipState *shipState);
	void tick();
	
	GameState currentGameState();
	bool isNewState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
