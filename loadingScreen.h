#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "guiManager.h"
#include "networkingManager.h"
#include <math.h>
#include <sstream>

class LoadingScreen : public IMenuScreen
{
    private:
        InputState *inputState;
        GuiManager *guiMgr;

        CollaborationInfo *myInfo;
        CollaborationInfo *pilotInfo;
        CollaborationInfo *navigatorInfo;
        CollaborationInfo *engineerInfo;

        bool isVisible;
        bool isEnd;
        NetworkingManager *networkingMgr;

        int progress;

        float wpixel;
        float hpixel;

        CEGUI::FrameWindow *loadingBackground;
        CEGUI::Editbox *indicator;
        CEGUI::FrameWindow *instructions;

        void render();
        
    public:
        void tick();
        bool end();

        void show();
        int getProgress();
        void updateProgress(int progress);
        void hide();

        void setInfos(CollaborationInfo *myInfo,
                      CollaborationInfo *pilotInfo,
                      CollaborationInfo *engineerInfo,
                      CollaborationInfo *navigatorInfo);

        bool visible();

        MenuType nextMenu();

        LoadingScreen(InputState *inputState, GuiManager *guiManager,
                      NetworkingManager *networkingMgr);

        ~LoadingScreen();
};

#endif
