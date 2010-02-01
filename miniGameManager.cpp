
#include "miniGameManager.h"
#include "testMiniGame.h"

void MiniGameManager::tick()
{
    if(currentMiniGame != NULL) {
    
        currentMiniGame->tick();
        
        if(currentMiniGame->end()) {
            currentMiniGame->getOverlay()->hide();
            currentMiniGame = NULL;
            shipControls->setEnabled(true);
        }
    }

    if(currentMiniGame == NULL) {
        if(inputState->isKeyDown(OIS::KC_1)) {
            currentMiniGame = new TestMiniGame(inputState, sceneManager);
            
            currentMiniGame->getOverlay()->show();
            
            shipControls->setEnabled(false);
        }
    } 
}
    
MiniGameManager::MiniGameManager(InputState *inputState, ShipControls *shipControls, SceneManager *sceneManager)
    : currentMiniGame(NULL)
    , inputState(inputState)
    , shipControls(shipControls)
    , sceneManager(sceneManager)
{
    
}
