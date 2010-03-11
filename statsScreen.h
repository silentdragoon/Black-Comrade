#ifndef STATS_SCREEN_H
#define STATS_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include <math.h>

class StatsScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        MenuType nextMenu();

        StatsScreen(InputState *inputState, GuiManager *guiManager);
        ~StatsScreen();
};

#endif
