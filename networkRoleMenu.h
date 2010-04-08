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
#include <sstream>
#include "serverInfo.h"

class NetworkRoleMenu : public IMenuScreen
{
    private:
        InputState *inputState;
        NetworkingManager *networkingMgr;
        GuiManager *guiMgr;

        CEGUI::Imageset *imageSet;
        CEGUI::Editbox *nameBox;
        CEGUI::PushButton *btn;

        bool isVisible;
        bool isEnd;

        int selectedGame;
        int gameRefreshDelay;
        int lastRefresh;

        int lastKey;
        int keyDelay;

        std::vector<ServerInfo*> servers;

        bool joinAGame();

        void handleInput();
        void handleKeys();
        void handleGameList();
        
    public:
        void tick();
        bool end();

        void show();
        void hide();

        void refreshGameList();

        bool createClicked(const CEGUI::EventArgs& e);

        bool visible();

        MenuType nextMenu();

        NetworkRoleMenu(InputState *inputState,NetworkingManager *networkingMgr, GuiManager *guiMgr);
        ~NetworkRoleMenu();
};

#endif
