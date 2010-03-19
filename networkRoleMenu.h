#ifndef NETWORK_ROLE_MENU_H
#define NETWORK_ROLE_MENU_H

#include "IMenuScreen.h"
#include "inputState.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "guiManager.h"
#include "menuType.h"
#include <OIS/OIS.h>
#include <math.h>
#include <vector>

class NetworkRoleMenu : public IMenuScreen
{
    private:
        InputState *inputState;
        NetworkingManager *networkingMgr;
        GuiManager *guiMgr;

        bool isVisible;
        bool isEnd;

        int gameRefreshDelay;
        int lastRefresh;

        std::vector<string> servers;
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        void refreshGameList();

        bool visible();

        MenuType nextMenu();

        NetworkRoleMenu(InputState *inputState,NetworkingManager *networkingMgr, GuiManager *guiMgr);
        ~NetworkRoleMenu();
};

#endif
