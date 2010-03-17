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
#include "collaborationInfo.h"

#include "statsScreen.h"

class PostGame : public IExit, public ITickable {
private:

    SceneManager *sceneMgr;
    GuiManager *guiMgr;
    InputState *inputState;
    NetworkingManager *networkingMgr;
    Ogre::RenderWindow *window;
    StateUpdate *postGameLoop;
    CollaborationInfo *pilotInfo;
    CollaborationInfo *navInfo;
    CollaborationInfo *engInfo;

    void clearMenuUI();

    void loadNextMenu();

    IMenuScreen *currentMenuScreen;
    IMenuScreen *statsScreen;


public:
    PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
            InputState *inputState,GuiManager *guiMgr,
            NetworkingManager *networkingMgr,
            CollaborationInfo *pilotInfo,
            CollaborationInfo *navInfo,
            CollaborationInfo *engInfo);

    void run();

    void tick();
    void exit();
};
