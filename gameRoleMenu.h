#ifndef GAME_ROLE_MENU_H
#define GAME_ROLE_MENU_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "guiManager.h"
#include "menuType.h"
#include <OIS/OIS.h>
#include <math.h>

class GameRoleMenu : public IMenuScreen
{
    private:
        InputState *inputState;
        NetworkingManager *networkingMgr;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        void updateOptions();
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        bool visible();

        MenuType nextMenu();

        GameRoleMenu(InputState *inputState,NetworkingManager *networkingMgr, GuiManager *guiMgr);
        ~GameRoleMenu();
};

#endif
