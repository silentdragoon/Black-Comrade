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
    bgImageSet = &(CEGUI::ImagesetManager::getSingleton().create("lobby.xml"));
    btImageSet = &(CEGUI::ImagesetManager::getSingleton().create("buttons.xml"));

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
    if (lastKey < keyDelay || nameBox->isActive()) {
        lastKey ++;
        return;
    }

    if (inputState->isKeyDown(OIS::KC_D)) {
        lastKey = 0;
        bool hosted = networkingMgr->hostGame("",true);
        if (hosted) isEnd = true;
    } else if (inputState->isKeyDown(OIS::KC_S)) {
        lastKey = 0;
        bool hosted = networkingMgr->hostGame(nameBox->getText().c_str(),false);
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

bool NetworkRoleMenu::createClicked(const CEGUI::EventArgs& e) {
    bool hosted = networkingMgr->hostGame(nameBox->getText().c_str(),false);
    if (hosted) {
        isEnd = true;
        networkingMgr->discoveryAgent->destroyClient();
    }
    return true;
}

bool NetworkRoleMenu::listDoubleClicked(const CEGUI::EventArgs& e) {
    bool joined = joinAGame();
    if (joined) {
        isEnd = true;
        networkingMgr->discoveryAgent->destroyClient();
    } else {
        // Could not join selected game
        std::cout << "failed" << std::endl;
    }
}

bool NetworkRoleMenu::listSelectionChanged(const CEGUI::EventArgs& e) {
    if (gameList->getFirstSelectedItem() == NULL) return false;

    selectedGame = (int) gameList->getItemRowIndex(gameList->getFirstSelectedItem());

    return true;
}

void NetworkRoleMenu::handleGameList() {
    if (lastRefresh == gameRefreshDelay && !isEnd) {
        // Look for more games
        networkingMgr->discoveryAgent->startServerListUpdate(6001);
        lastRefresh = 0;
    } else { lastRefresh ++; }

    if (!isEnd) networkingMgr->discoveryAgent->updateServerList();
    refreshGameList();
}

void NetworkRoleMenu::refreshGameList() {
   
    servers = networkingMgr->discoveryAgent->getServerList();
    gameList->resetList();
    for(std::vector<ServerInfo*>::const_iterator it=servers.begin();it!=servers.end(); ++it) {
        ServerInfo *server =  *it;
        //server->print();

        // Don't list games which are not responding (possibly full)
        if (server->getLastPong() > 0) continue;
        addGameToList(server);
    }

    if (selectedGame > (int) gameList->getRowCount()  -1) return;

    CEGUI::MCLGridRef ref = CEGUI::MCLGridRef(selectedGame,0);
    gameList->setItemSelectState(gameList->getItemAtGridReference(ref),true);
}

void NetworkRoleMenu::addGameToList(ServerInfo *game) {
    CEGUI::ListboxItem *listboxitem;
    int row;

    std::stringstream latency;
    latency << game->getLatency();

    // TODO: Show the roles as images
    std::stringstream roles;
    std::string pilot = (game->isPilotTaken()) ? " " : "P";
    std::string eng = (game->isEngineerTaken()) ? " " : "E";
    std::string nav = (game->isNavigatorTaken()) ? " " : "N";

    roles << pilot << " " << eng << " " << nav;

    listboxitem = new MyListItem(game->getName());
    row = gameList->addRow(listboxitem, 0);
    listboxitem = new MyListItem(roles.str());
    gameList->setItem(listboxitem, 1, row);
    listboxitem = new MyListItem(latency.str());
    gameList->setItem(listboxitem, 2, row);
}

bool NetworkRoleMenu::joinAGame() {
    try {
        ServerInfo *game = servers.at(selectedGame);
        if (game->getLastPong() > 1) return false;
        return networkingMgr->connectToGame(game);
    } catch (...) { return false; }
}

MenuType::NetworkRoleMenu::nextMenu() {
    return MT_CHOOSE_GAME_ROLE;
}

void NetworkRoleMenu::show() {
    // Show background image etc
    if (isVisible) return;

    guiMgr->addStaticImage("Lobby",0.5, 0.5,1.0, 1.0,"Lobby","Whole");
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float wpixel = winWidth / 1680.0;
    float hpixel = winHeight / 1050.0;

    gameList = static_cast<CEGUI::MultiColumnList *>
               (CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/MultiColumnList","gameList"));

    nameBox = static_cast<CEGUI::Editbox*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/IEditbox","nameBox"));
    guiMgr->getRootWindow()->addChildWindow(nameBox);
    guiMgr->getRootWindow()->addChildWindow(gameList);

    CEGUI::Image namePlacement = bgImageSet->getImage("GameNamePlacement");
    float nameWidth =  200 * wpixel;
    float nameHeight =  22 * hpixel;
    float nameX =  namePlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float nameY =  namePlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    CEGUI::Image listPlacement = bgImageSet->getImage("TablePlacement");
    float listWidth =  listPlacement.getWidth() * wpixel;
    float listHeight =  listPlacement.getHeight() * wpixel;
    float listX =  listPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float listY =  listPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    nameBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,nameWidth),CEGUI::UDim(0,nameHeight)));
    nameBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0,nameX),CEGUI::UDim(0,nameY)));
    nameBox->setMaxTextLength(20);

    // TODO: Set the size and the position of the game list correctly
    gameList->setSize(CEGUI::UVector2(CEGUI::UDim(0,listWidth),CEGUI::UDim(0,listHeight)));
    gameList->setPosition(CEGUI::UVector2(CEGUI::UDim(0,listX),CEGUI::UDim(0,listY)));
    gameList->addColumn("Name", 0, CEGUI::UDim(0.5,0));
    gameList->addColumn("Roles available", 1, CEGUI::UDim(0.3,0));
    gameList->addColumn("Latency", 2, CEGUI::UDim(0.15,0));
    gameList->setSelectionMode(CEGUI::MultiColumnList::RowSingle);
    gameList->setUserSortControlEnabled(false);
    gameList->setUserColumnSizingEnabled(false);
    gameList->setUserColumnDraggingEnabled(false);

    std::stringstream out;
    out << networkingMgr->nick << "'s Game";
    nameBox->setText(out.str());
    nameBox->setCaratIndex(nameBox->getText().length());

    CEGUI::Image buttonPlacement = bgImageSet->getImage("CreateButton");
    float buttonWidth =  buttonPlacement.getWidth() * wpixel;
    float buttonHeight =  buttonPlacement.getHeight() * wpixel;
    float buttonX =  buttonPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float buttonY =  buttonPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    btn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/ImageButton"));
    guiMgr->getRootWindow()->addChildWindow(btn);
    btn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,buttonX),CEGUI::UDim(0,buttonY)));
    btn->setSize(CEGUI::UVector2(CEGUI::UDim(0,buttonWidth),CEGUI::UDim(0,buttonHeight)));
    btn->setProperty(CEGUI::String("NormalImage"),CEGUI::String("set:Buttons image:Create"));
    btn->setProperty(CEGUI::String("HoverImage"),CEGUI::String("set:Buttons image:CreateOver"));
    btn->setProperty(CEGUI::String("PushedImage"),CEGUI::String("set:Buttons image:CreateClick"));

    btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&NetworkRoleMenu::createClicked, this));
    gameList->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&NetworkRoleMenu::listDoubleClicked, this));
    gameList->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber(&NetworkRoleMenu::listSelectionChanged, this));

    isVisible = true;
}

void NetworkRoleMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("Lobby");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(nameBox);
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(btn);
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow(gameList);
}

bool NetworkRoleMenu::end() { return isEnd; }

bool NetworkRoleMenu::visible() { return isVisible; }
