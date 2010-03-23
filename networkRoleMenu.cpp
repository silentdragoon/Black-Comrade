#include "networkRoleMenu.h"
#include "IMenuScreen.h"

NetworkRoleMenu::NetworkRoleMenu(InputState *inputState,
                                 NetworkingManager *networkingMgr,
                                 GuiManager *guiMgr)
    : inputState(inputState)
    , networkingMgr(networkingMgr)
    , guiMgr(guiMgr)
    , isVisible(false)
    , isEnd(false)
    , selectedGame(0)
    , keyDelay(10)
    , lastKey(0)
{
    CEGUI::ImagesetManager::getSingleton().create("NetworkRoleMenu.imageset");
    gameRefreshDelay = 100;
    lastRefresh = gameRefreshDelay;
}

void NetworkRoleMenu::tick() {
    // Check for key presses etc
    handleInput();
    handleGameList();
}

void NetworkRoleMenu::handleInput() {
    handleKeys();
}

void NetworkRoleMenu::handleKeys() {
    if (lastKey < keyDelay) {
        lastKey ++;
        return;
    }

    if (inputState->isKeyDown(OIS::KC_D)) {
        lastKey = 0;
        bool hosted = networkingMgr->hostGame(true);
        if (hosted) isEnd = true;
    } else if (inputState->isKeyDown(OIS::KC_S)) {
        lastKey = 0;
        bool hosted = networkingMgr->hostGame(false);
        if (hosted) {
            isEnd = true;
            networkingMgr->discoveryAgent->destroyClient();
        }
    } else if (inputState->isKeyDown(OIS::KC_C)) {
        lastKey = 0;
        bool joined = joinAGame();
        if (joined) {
            isEnd = true;
            networkingMgr->discoveryAgent->destroyClient();
        } else {
            // Could not join selected game
            std::cout << "failed" << std::endl;
        }
    } else if (inputState->isKeyDown(OIS::KC_UP)) {
        lastKey = 0;
        selectedGame --;
        std::cout << "Selected game: " << selectedGame << "\n";
    } else if (inputState->isKeyDown(OIS::KC_DOWN)) {
        lastKey = 0;
        selectedGame ++;
        std::cout << "Selected game: " << selectedGame << "\n";
    } else if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(-1);
    }
}

void NetworkRoleMenu::handleGameList() {
    if (lastRefresh == gameRefreshDelay && !isEnd) {
        // Refresh games
        refreshGameList();
        lastRefresh = 0;
    } else { lastRefresh ++; }

    if (!isEnd) networkingMgr->discoveryAgent->updateServerList();
}

void NetworkRoleMenu::refreshGameList() {
    std::cout << "Refreshing game list\n";
    networkingMgr->discoveryAgent->startServerListUpdate(6001);
    servers = networkingMgr->discoveryAgent->getServerList();
    for(std::vector<ServerInfo*>::const_iterator it=servers.begin();it!=servers.end(); ++it) {
        // TODO: List these on the screen, show selection etc
        ServerInfo *server =  *it;
        server->print();
    }
}

bool NetworkRoleMenu::joinAGame() {
    try {
        ServerInfo *game = servers.at(selectedGame);
        return networkingMgr->connectToGame(game);
    } catch (...) { return false; }
}

MenuType::NetworkRoleMenu::nextMenu() {
    return MT_CHOOSE_GAME_ROLE;
}

void NetworkRoleMenu::show() {
    // Show background image etc
    if (isVisible) return;

    guiMgr->addStaticImage("NetworkRoleMenu",0.5, 0.5,1.0, 1.0,"NetworkRoleMenu","NetworkRoleMenu");
    isVisible = true;
}

void NetworkRoleMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NetworkRoleMenu");
}

bool NetworkRoleMenu::end() { return isEnd; }

bool NetworkRoleMenu::visible() { return isVisible; }
