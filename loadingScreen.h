#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include <math.h>
#include <sstream>

class LoadingScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        int progress;

        float wpixel;
        float hpixel;
        CEGUI::Editbox *indicator;

        void render();
        
    public:
        void tick();
        bool end();

        void show();
        int getProgress();
        void updateProgress(int progress);
        void hide();

        bool visible();

        MenuType nextMenu();

        LoadingScreen(InputState *inputState, GuiManager *guiManager);
        ~LoadingScreen();
};

#endif
