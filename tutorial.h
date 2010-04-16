#ifndef TUTORIAL
#define TUTORIAL

#include "ITickable.h"
#include "guiManager.h"
#include "hud.h"
#include "miniGameManager.h"
#include "collaborationInfo.h"
#include "door.h"

#include <iostream>

enum TutorialState { TS_PRE, TS_START, TS_OPEN_CONSOLE, TS_CLOSE_CONSOLE, TS_WAITING_FOR_OTHERS, TS_END };

class Tutorial : public ITickable {
    private:

        GuiManager *guiMgr;
        HUD *hud;
        TutorialState state;
        MiniGameManager *miniGameMgr;
        CollaborationInfo *tutee;

        CollaborationInfo *tutee1;
        CollaborationInfo *tutee2;
        CollaborationInfo *tutee3;

        Door *door;

        int pauseProgress;

        void checkConsole();
        void checkForCompletion();

        void changeWithPause(TutorialState newState, int pause = 150);

    public:
        TutorialState getState();
        Tutorial(CollaborationInfo *tutee,
                 CollaborationInfo *tutee1, CollaborationInfo *tutee2, CollaborationInfo *tutee3,
                 GuiManager *guiMgr, HUD *hud, MiniGameManager *miniGameMgr, Door *door);
	virtual void tick();
};

#endif
