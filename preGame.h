#include <OGRE/Ogre.h>

#include "preGameMenu.h"
#include "guiManager.h"
#include "inputState.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "gameRole.h"
#include "stateUpdate.h"
#include "IExit.h"
#include "ITickable.h"
#include "mapManager2.h"

#include "networkRoleMenu.h"
#include "gameRoleMenu.h"
#include "loadingScreen.h"

class PreGame : public IExit, public ITickable {
private:

    PreGameMenu currentMenu;
    SceneManager *sceneMgr;
    GuiManager *guiMgr;
    InputState *inputState;
    NetworkingManager *networkingMgr;
    Ogre::RenderWindow *window;
    StateUpdate *preGameLoop;

    void clearMenuUI();

    void loadNextMenu();

    IMenuScreen *currentMenuScreen;
    IMenuScreen *networkRoleMenu;
    IMenuScreen *gameRoleMenu;
    IMenuScreen *loadingScreen;


public:
    PreGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
            InputState *inputState,GuiManager *guiMgr,
            NetworkingManager *networkingMgr);

    CollaborationInfo *run();

    MapManager* loadGame();

    void tick();
    void exit();
};
