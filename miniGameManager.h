
#ifndef MINI_GAME_MANAGER_H
#define MINI_GAME_MANAGER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "inputState.h"
#include "gunnerControls.h"
#include "IMiniGame.h"
#include "IConsoleInputReceiver.h"
#include "console.h"
#include "consoleShell.h"
#include "quickTimeMiniGame.h"
#include "sheildMiniGame.h"
#include "collaborationInfo.h"
#include "IExit.h"

using namespace Ogre;
using namespace std;

class MiniGameManager : public ITickable, public OIS::KeyListener {
private:
    
    GunnerControls *playerControls;
    InputState *inputState;
    IConsoleInputReceiver *inputReceiver;
    SceneManager *sceneManager;
    Console *console;
    ConsoleShell *consoleShell;
    CollaborationInfo *player;
    IExit *exit;
    std::map <std::string,int> *difficulties;

    CollaborationInfo *otherPlayer1;
    CollaborationInfo *otherPlayer2;

    bool consoleBeenOpened;
    bool consoleBeenClosed;

    int lastKey;
    int keyDelay;
    bool releasedKey;
    
    IMiniGame *currentMiniGame;

    void resetScores();

    void handleKeys();
    void setConsoleState(bool isOpen);
    void setInitialDifficulties();
    void setDifficulty(IMiniGame *game, int difficulty);
    int getDifficulty(IMiniGame *game);
    void increaseDifficulty(IMiniGame *game);

    void endGame();
    
public:
    bool aKeyPressed;

    void tick();
    
    MiniGameManager(Console *console,
                    InputState *inputState, GunnerControls *playerControls,
                    SceneManager *sceneManager, CollaborationInfo *player,
                    CollaborationInfo *otherPlayer1, CollaborationInfo *otherPlayer2,
                    IExit *exit);

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);

    bool hasConsoleBeenOpened();
    bool hasConsoleBeenClosed();
    bool isGameInProgress();
};

#endif
