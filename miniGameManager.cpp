
#include "miniGameManager.h"

MiniGameManager::MiniGameManager(Console *console,
                                 InputState *inputState, GunnerControls *playerControls,
                                 SceneManager *sceneManager,
                                 CollaborationInfo *player,
                                 IExit *exit)
    : currentMiniGame(NULL)
    , console(console)
    , inputState(inputState)
    , inputReceiver(NULL)
    , playerControls(playerControls)
    , sceneManager(sceneManager)
    , player(player)
    , aKeyPressed(false)
    , exit(exit)
{
    consoleShell = new ConsoleShell(console,inputState,exit);
    inputReceiver = consoleShell;
}

void MiniGameManager::tick()
{
    player->toRepair = SS_NONE;
    player->repairAmount = 0;

    if (console->getVisible()) {
        if (inputState->isKeyDown(OIS::KC_F2)) {
            setConsoleState(false);
            return;
        }
    } else {
        if (inputState->isKeyDown(OIS::KC_F1)) {
            setConsoleState(true);
        } else { return; }
    }

    if (currentMiniGame != NULL) {
        inputReceiver = currentMiniGame;
        currentMiniGame->tick();
        if (currentMiniGame->end()) {
            std::cout << "Ended minigame with score "
                      << currentMiniGame->getScore()
                      << std::endl;
            delete currentMiniGame;
            currentMiniGame = NULL;
            inputReceiver = consoleShell;
            consoleShell->showPrompt();
        } else {
            // Game has not ended, so get the score for this tick
            player->toRepair = currentMiniGame->getSystem();
            player->repairAmount = currentMiniGame->getScore();
        }
    } else {
        IMiniGame *gameToPlay = consoleShell->getGameToPlay();
        if (gameToPlay != NULL) {
            currentMiniGame = gameToPlay;
            console->clearPrompt();
        }
        consoleShell->tick();
    }
}

void MiniGameManager::setConsoleState(bool isVisible) {
    playerControls->setEnabled(!isVisible);
    console->setVisible(isVisible);
    if (isVisible)
        inputState->addKeyListener(this);
    else
        inputState->clearKeyListener();
}

bool MiniGameManager::keyPressed(const OIS::KeyEvent &arg) {
    if (inputReceiver == NULL) return false;

    if (arg.key == OIS::KC_RETURN) {
        inputReceiver->returnKeyPressed();
        aKeyPressed = true;
        return true;
    } else if (arg.key == OIS::KC_BACK) {
        inputReceiver->backspaceKeyPressed();
        aKeyPressed = true;
        return true;
    } else if (arg.text == 0) {
        inputReceiver->otherKeyPressed(arg);
        return true;
    }

    char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
    for(int c=0;c<sizeof(legalchars);c++){
        if(legalchars[c]==arg.text){
            inputReceiver->alphaNumKeyPressed(arg);
            aKeyPressed = true;
            break;
        }
    }
    return true;
}

bool MiniGameManager::keyReleased(const OIS::KeyEvent &arg) { return false; }

