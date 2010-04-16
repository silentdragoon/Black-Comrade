#ifndef TUTORIAL
#define TUTORIAL

#include "ITickable.h"
#include "guiManager.h"
#include "hud.h"
#include "miniGameManager.h"
#include "collaborationInfo.h"
#include "door.h"

enum TutorialState { TS_START, TS_OPEN_CONSOLE, TS_CLOSE_CONSOLE, TS_END };

class Tutorial : public ITickable {
    private:

        GuiManager *guiMgr;
        HUD *hud;
        TutorialState state;
        MiniGameManager *miniGameMgr;
        CollaborationInfo *tutee;
        Door *door;

        void checkConsole();

    public:
        TutorialState getState();
        Tutorial(CollaborationInfo *tutee, GuiManager *guiMgr, 
                 HUD *hud, MiniGameManager *miniGameMgr, Door *door);
	virtual void tick();
};

#endif
