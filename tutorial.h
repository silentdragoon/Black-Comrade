#ifndef TUTORIAL
#define TUTORIAL

#include "ITickable.h"
#include "guiManager.h"
#include "hud.h"
#include "miniGameManager.h"
#include "collaborationInfo.h"

enum TutorialState { TS_START, TS_END };

class Tutorial : public ITickable {
    private:

        GuiManager *guiMgr;
        HUD *hud;
        TutorialState state;
        MiniGameManager *miniGameMgr;
        CollaborationInfo *tutee;

    public:
        TutorialState getState();
        Tutorial(CollaborationInfo *tutee, GuiManager *guiMgr, 
                 HUD *hud, MiniGameManager *miniGameMgr);
	virtual void tick();
};

#endif
