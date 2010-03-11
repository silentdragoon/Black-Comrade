#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include <math.h>

class LoadingScreen : public IMenuScreen
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

        PreGameMenu nextMenu();

        LoadingScreen(InputState *inputState, GuiManager *guiManager);
        ~LoadingScreen();
};

#endif
