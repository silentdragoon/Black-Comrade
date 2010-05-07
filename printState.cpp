
#include "printState.h"

PrintState::PrintState(GameStateMachine *gameStateMachine)
	: gameStateMachine(gameStateMachine)
{}

void PrintState::tick()
{
	if(gameStateMachine->isNewState()) {
		
		cout << "Changed to state: ";
		
		switch(gameStateMachine->currentGameState())
		{
            case GS_TUTORIAL:
                cout << "Tutorial\n";
                break;
			case GS_STEALTH:
				cout << "Stealth\n";
				break;
			case GS_ATTACK:
				cout << "Attack\n";
				break;
			case GS_FLEE:
				cout << "Flee\n";
				break;
			case GS_GAME_OVER:
				cout << "Game Over\n";
				break;
			case GS_END:
				cout << "End\n";
				break;
		}
	}
}
