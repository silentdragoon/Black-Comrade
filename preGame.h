#include <OGRE/Ogre.h>

#include "menuType.h"
#include "guiManager.h"
#include "inputState.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "gameRole.h"
#include "stateUpdate.h"
#include "IExit.h"
#include "ITickable.h"

#include "storyMenu.h"
#include "networkRoleMenu.h"
#include "gameRoleMenu.h"
#include "loadingScreen.h"

class PreGame : public IExit, public ITickable, public OIS::KeyListener {
private:

    SceneManager *sceneMgr;
    GuiManager *guiMgr;
    InputState *inputState;
    NetworkingManager *networkingMgr;
    Ogre::RenderWindow *window;
    StateUpdate *preGameLoop;

    void clearMenuUI();

    void loadNextMenu();

    IMenuScreen *currentMenuScreen;
    IMenuScreen *storyMenu;
    IMenuScreen *networkRoleMenu;
    IMenuScreen *gameRoleMenu;
    LoadingScreen *loadingScreen;

    void render();
    void handleInput();
    void handleKeys();
    void handleMouse();


public:
    PreGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
            InputState *inputState,GuiManager *guiMgr,
            NetworkingManager *networkingMgr);

    CollaborationInfo *showMenus();

    void hideLoadingScreen();
    LoadingScreen *getLoadingScreen();

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);

    void tick();
    void exit();
};
