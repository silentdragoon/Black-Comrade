
#include "miniGameManager.h"
#include "testMiniGame.h"

void MiniGameManager::tick()
{
    if(currentMiniGame != NULL) {
    
        currentMiniGame->tick();
        
        if(currentMiniGame->end()) {
            currentMiniGame->getOverlay()->hide();
            currentMiniGame = NULL;
            pilotControls->setEnabled(true);
        }
    }

    if(currentMiniGame == NULL) {
        if(inputState->isKeyDown(OIS::KC_1)) {
            currentMiniGame = new TestMiniGame(inputState, sceneManager);
            
            currentMiniGame->getOverlay()->show();
            
            pilotControls->setEnabled(false);
        }
    } 
}
    
MiniGameManager::MiniGameManager(InputState *inputState, PilotControls *pilotControls, SceneManager *sceneManager)
    : currentMiniGame(NULL)
    , inputState(inputState)
    , pilotControls(pilotControls)
    , sceneManager(sceneManager)
{
    
}
