#include <OGRE/Ogre.h>

#include "menuSystem.h"
#include "networkRole.h"
#include "gameRole.h"
#include "stateUpdate.h"
#include "IExit.h"
#include "ITickable.h"
#include "collaborationInfo.h"
#include "soundManager.h"

#include "statsScreen.h"

class PostGame : public ITickable, public MenuSystem {
private:

    SoundManager *soundMgr;

    CollaborationInfo *pilotInfo;
    CollaborationInfo *navInfo;
    CollaborationInfo *engInfo;

    void loadNextMenu();

    IMenuScreen *statsScreen;

public:
    PostGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
            InputState *inputState,GuiManager *guiMgr,
            SoundManager *soundMgr,
            CollaborationInfo *pilotInfo,
            CollaborationInfo *navInfo,
            CollaborationInfo *engInfo,
            GameState finishState);

    void showMenus();
    void tick();
};
