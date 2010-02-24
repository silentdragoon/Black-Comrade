
#include "miniGameManager.h"
#include "consoleMiniGame.h"

void MiniGameManager::tick()
{
    if(currentMiniGame != NULL) {
    
        currentMiniGame->tick();
        
        if(currentMiniGame->end()) {
            std::cout << "Ending console minigame with score "
                      << currentMiniGame->getScore() << std::endl;
            delete currentMiniGame;

            currentMiniGame = NULL;
            playerControls->setEnabled(true);

        }
    }

    if(currentMiniGame == NULL) {
        if(inputState->isKeyDown(OIS::KC_1)) {
            currentMiniGame = new ConsoleMiniGame(console, inputState);
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
