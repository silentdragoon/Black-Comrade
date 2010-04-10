#include "gameRoleMenu.h"

GameRoleMenu::GameRoleMenu(InputState *inputState,
                           NetworkingManager *networkingMgr,
                           GuiManager *guiMgr)
    : inputState(inputState)
    , networkingMgr(networkingMgr)
    , guiMgr(guiMgr)
    , isVisible(false)
    , isEnd(false)
{
    bgImageSet = &(CEGUI::ImagesetManager::getSingleton().create("chooseplayer.xml"));
    btImageSet = &(CEGUI::ImagesetManager::getSingleton().create("buttons.xml"));
}

void GameRoleMenu::tick() {
    // Check for key presses etc

    if (networkingMgr->lobby->roleOptionsChanged) {
        // Present the user with the current options
        updateOptions();
    }

    networkingMgr->tick();

    if (networkingMgr->collabInfo->getGameRole() != NO_GAME_ROLE) {
        isEnd = true;
    }

    if (inputState->isKeyDown(OIS::KC_P)) {
        networkingMgr->lobby->chooseGameRole(PILOT);
    } else if (inputState->isKeyDown(OIS::KC_N)) {
        networkingMgr->lobby->chooseGameRole(NAVIGATOR);
    } else if (inputState->isKeyDown(OIS::KC_E))	 {
        networkingMgr->lobby->chooseGameRole(ENGINEER);
    } else if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(0);
    }
}

void GameRoleMenu::updateOptions() {

    pilBtn->hide();
    engBtn->hide();
    navBtn->hide();

    switch (networkingMgr->lobby->getChosenGameRole()) {
        case NO_GAME_ROLE:
            break;
        case PILOT:
            pilBtn->disable();
            pilBtn->show();
            return;
        case NAVIGATOR:
            navBtn->disable();
            navBtn->show();
            return;
        case ENGINEER:
            engBtn->disable();
            engBtn->show();
            return; 
    }

    if (!networkingMgr->lobby->pilotTaken)
        pilBtn->show();
    if (!networkingMgr->lobby->engTaken)
        engBtn->show();
    if (!networkingMgr->lobby->navTaken)
        navBtn->show();
}

bool GameRoleMenu::pilotClicked(const CEGUI::EventArgs& e) {

    networkingMgr->lobby->chooseGameRole(PILOT);
    return true;
}

bool GameRoleMenu::engineerClicked(const CEGUI::EventArgs& e) {

    networkingMgr->lobby->chooseGameRole(ENGINEER);
    return true;
}

bool GameRoleMenu::navigatorClicked(const CEGUI::EventArgs& e) {

    networkingMgr->lobby->chooseGameRole(NAVIGATOR);
    return true;
}

MenuType GameRoleMenu::nextMenu() {
    return MT_LOADING;
}

void GameRoleMenu::show() {
    // Show background image etc
    if (isVisible) return;

    guiMgr->addStaticImage("ChoosePlayer",0.5, 0.5,1.0, 1.0,"ChoosePlayer","Whole");

    setUpButtons();

    isVisible = true;
}

void GameRoleMenu::setUpButtons() {
    int winWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    int winHeight= Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
    float wpixel = winWidth / 1680.0;
    float hpixel = winHeight / 1050.0;

    CEGUI::Image buttonPlacement = bgImageSet->getImage("PilButton");
    float buttonWidth =  buttonPlacement.getWidth() * wpixel;
    float buttonHeight =  buttonPlacement.getHeight() * wpixel;

    float buttonX = buttonPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    float buttonY = buttonPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    pilBtn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/ImageButton"));
    guiMgr->getRootWindow()->addChildWindow(pilBtn);
    pilBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,buttonX),CEGUI::UDim(0,buttonY)));
    pilBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0,buttonWidth),CEGUI::UDim(0,buttonHeight)));
    pilBtn->setProperty(CEGUI::String("NormalImage"),CEGUI::String("set:Buttons image:Pilot"));
    pilBtn->setProperty(CEGUI::String("HoverImage"),CEGUI::String("set:Buttons image:PilotOver"));
    pilBtn->setProperty(CEGUI::String("PushedImage"),CEGUI::String("set:Buttons image:PilotClick"));
    pilBtn->setProperty(CEGUI::String("DisabledImage"),CEGUI::String("set:Buttons image:PilotOver"));

    buttonPlacement = bgImageSet->getImage("EngButton");
    buttonX = buttonPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    buttonY = buttonPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    engBtn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/ImageButton"));
    guiMgr->getRootWindow()->addChildWindow(engBtn);
    engBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,buttonX),CEGUI::UDim(0,buttonY)));
    engBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0,buttonWidth),CEGUI::UDim(0,buttonHeight)));
    engBtn->setProperty(CEGUI::String("NormalImage"),CEGUI::String("set:Buttons image:Engineer"));
    engBtn->setProperty(CEGUI::String("HoverImage"),CEGUI::String("set:Buttons image:EngineerOver"));
    engBtn->setProperty(CEGUI::String("PushedImage"),CEGUI::String("set:Buttons image:EngineerClick"));
    engBtn->setProperty(CEGUI::String("DisabledImage"),CEGUI::String("set:Buttons image:EngineerOver"));

    buttonPlacement = bgImageSet->getImage("NavButton");
    buttonX = buttonPlacement.getSourceTextureArea().getPosition().d_x * wpixel;
    buttonY = buttonPlacement.getSourceTextureArea().getPosition().d_y * hpixel;

    navBtn = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingletonPtr()->createWindow("BlackComrade/ImageButton"));
    guiMgr->getRootWindow()->addChildWindow(navBtn);
    navBtn->setPosition(CEGUI::UVector2(CEGUI::UDim(0,buttonX),CEGUI::UDim(0,buttonY)));
    navBtn->setSize(CEGUI::UVector2(CEGUI::UDim(0,buttonWidth),CEGUI::UDim(0,buttonHeight)));
    navBtn->setProperty(CEGUI::String("NormalImage"),CEGUI::String("set:Buttons image:Navigator"));
    navBtn->setProperty(CEGUI::String("HoverImage"),CEGUI::String("set:Buttons image:NavigatorOver"));
    navBtn->setProperty(CEGUI::String("PushedImage"),CEGUI::String("set:Buttons image:NavigatorClick"));
    navBtn->setProperty(CEGUI::String("DisabledImage"),CEGUI::String("set:Buttons image:NavigatorOver"));

    pilBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameRoleMenu::pilotClicked, this));
    engBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameRoleMenu::engineerClicked, this));
    navBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameRoleMenu::navigatorClicked, this));

    pilBtn->hide();
    engBtn->hide();
    navBtn->hide();
}

void GameRoleMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("ChoosePlayer");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PilotRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("EngRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NavRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("ChosenRoleText");
}

bool GameRoleMenu::end() { return isEnd; }

bool GameRoleMenu::visible() { return isVisible; }
