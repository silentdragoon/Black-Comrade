#include "inputState.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "gameRole.h"

class PreGame {
private:
    InputState *inputState;
    NetworkingManager *networkingManager;
    CollaborationInfo *runLobby();

public:
    PreGame(InputState *inputState, NetworkingManager *networkingManager);
    CollaborationInfo *run();
    
};
