
#include "miniGameManager.h"
#include "testMiniGame.h"

void MiniGameManager::tick()
{
    if(currentMiniGame != NULL) {
    
        currentMiniGame->tick();
        
        if(currentMiniGame->end()) {
            currentMiniGame->getOverlay()->hide();
            std::cout << currentMiniGame->getScore() << std::endl;
            delete currentMiniGame;
            currentMiniGame = NULL;
            playerControls->setEnabled(true);

        }
    }

    if(currentMiniGame == NULL) {
        if(inputState->isKeyDown(OIS::KC_1)) {
            currentMiniGame = new TestMiniGame(inputState, sceneManager);
            
            currentMiniGame->getOverlay()->show();
            
            playerControls->setEnabled(false);
        }
    } 
}
    
MiniGameManager::MiniGameManager(Console *console, InputState *inputState, IPlayerControls *playerControls, SceneManager *sceneManager)
    : currentMiniGame(NULL)
    , console(console)
    , inputState(inputState)
    , playerControls(playerControls)
    , sceneManager(sceneManager)
{
    
}
