
#ifndef GAME_STATE_MACHINE_H
#define GAME_STATE_MACHINE_H

#include "ITickable.h"
#include "mapManager.h"
#include "shipState.h"

enum GameState { GS_STELPH, GS_ATTACK, GS_FLEE, GS_GAME_OVER, GS_END };

class GameStateMachine : public ITickable
{
private:
	GameState gameState;
	MapManager *mapManager;
	ShipState *shipState;
	bool mIsNewState;
	
public:
	GameStateMachine(MapManager *mapManager, ShipState *shipState);
	void tick();
	
	GameState currentGameState();
	bool isNewState();
};

#endif
