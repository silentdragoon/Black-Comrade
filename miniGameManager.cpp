
#include "miniGameManager.h"
#include "testMiniGame.h"

void MiniGameManager::tick()
{
    if(currentMiniGame == NULL) {
        if(keyState->isKeyDown(OIS::KC_1)) {
            currentMiniGame = new TestMiniGame();
            
            shipControls->setEnabled(false);
        }
    }
    
    if(currentMiniGame != NULL) {
        if(currentMiniGame->end()) {
            currentMiniGame = NULL;
            shipControls->setEnabled(true);
        }
    }

    if(currentMiniGame != NULL) {
        currentMiniGame->tick();
    }
}
    
MiniGameManager::MiniGameManager(KeyState *keyState, ShipControls *shipControls, SceneManager *sceneManager)
    : currentMiniGame(NULL)
    , keyState(keyState)
    , shipControls(shipControls)
    , sceneManager(sceneManager)
{
    
}
