#ifndef STATS_SCREEN_H
#define STATS_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include "collaborationInfo.h"
#include <math.h>

class StatsScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        CollaborationInfo *pilotInfo;
        CollaborationInfo *navInfo;
        CollaborationInfo *engInfo;
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        MenuType nextMenu();

        StatsScreen(InputState *inputState, GuiManager *guiManager,
                    CollaborationInfo *pilotInfo,
                    CollaborationInfo *navInfo,
                    CollaborationInfo *engInfo);

        ~StatsScreen();
};

#endif
