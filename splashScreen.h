#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include "menuType.h"
#include <OIS/OIS.h>

class SplashScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        CEGUI::FrameWindow *instructions;
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        MenuType nextMenu();

        SplashScreen(InputState *inputState, GuiManager *guiMgr);
        ~SplashScreen();
};

#endif
