
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
    , consoleBeenOpened(false)
    , consoleBeenClosed(false)
    , keyDelay(10)
    , lastKey(0)
    , releasedKey(true)
{
    difficulties = new std::map <std::string,int>();
    setInitialDifficulties();
    consoleShell = new ConsoleShell(console,inputState,difficulties,exit);
    inputReceiver = consoleShell;
}

void MiniGameManager::tick()
{
    player->toRepair = SS_NONE;
    player->repairAmount = 0;

    handleKeys();

    if (currentMiniGame != NULL) {
        inputReceiver = currentMiniGame;
        currentMiniGame->tick();
        if (currentMiniGame->end()) {
            console->makeBlank();
            consoleShell->showPrompt();
            std::cout << "Ended minigame\n";
            if (currentMiniGame->complete()) {
                increaseDifficulty(currentMiniGame);
                player->getPlayerStats()->repairsMade ++;
            }
            delete currentMiniGame;
            currentMiniGame = NULL;
            inputReceiver = consoleShell;
            consoleShell->showPrompt();
            player->repairing = false;
        } else {
            // Game has not ended, so get the score for this tick
            player->toRepair = currentMiniGame->getSystem();
            player->repairAmount = currentMiniGame->getScore();
            player->repairing = true;
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

void MiniGameManager::handleKeys() {
    if (lastKey < keyDelay) {
        lastKey ++;
        releasedKey = false;
        return;
    }
    if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        if (console->getVisible() && releasedKey) {
            // Hide it
            consoleBeenClosed = consoleBeenOpened;
            setConsoleState(false);
        } else if (!console->getVisible() && releasedKey) {
            // Show it
            setConsoleState(true);
            consoleBeenOpened = true;
        }
        releasedKey = false;
        lastKey = 0;
    } else {
        releasedKey = true;
    }
}

bool MiniGameManager::hasConsoleBeenOpened() { return consoleBeenOpened; }

bool MiniGameManager::hasConsoleBeenClosed() { return consoleBeenClosed; }

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
    } else if (arg.text == 0 || arg.key == OIS::KC_TAB) {
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

void MiniGameManager::increaseDifficulty(IMiniGame *game) {
     int current = getDifficulty(game);
     if (current < 3) setDifficulty(game,current+1);
}

bool MiniGameManager::keyReleased(const OIS::KeyEvent &arg) { return false; }

void MiniGameManager::setDifficulty(IMiniGame *game, int difficulty) {
    (*difficulties)[game->getName()] = difficulty;
}

int MiniGameManager::getDifficulty(IMiniGame *game) {
    try {
        int difficulty = (*difficulties)[game->getName()];
        return difficulty;
    } catch (...) { return 1; }
}

void MiniGameManager::setInitialDifficulties() {
    difficulties->insert(std::pair<std::string,int>("engineGame",1));
    difficulties->insert(std::pair<std::string,int>("shieldGame",1));
    difficulties->insert(std::pair<std::string,int>("weaponGame",1));
}

