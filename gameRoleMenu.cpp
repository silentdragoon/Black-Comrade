#include "gameRoleMenu.h"
#include "IMenuScreen.h"

GameRoleMenu::GameRoleMenu(InputState *inputState,
                           NetworkingManager *networkingMgr,
                           GuiManager *guiMgr)
    : inputState(inputState)
    , networkingMgr(networkingMgr)
    , guiMgr(guiMgr)
    , isVisible(false)
    , isEnd(false)
{
    CEGUI::ImagesetManager::getSingleton().create("GameRoleMenu.imageset");
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
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PilotRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("EngRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NavRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("ChosenRoleText");

    if (!networkingMgr->lobby->pilotTaken)
        guiMgr->addStaticText("PilotRoleText", "(P)ilot",0.5, 0.25, 1);
    if (!networkingMgr->lobby->engTaken)
        guiMgr->addStaticText("EngRoleText", "(E)ngineer",0.5, 0.5, 1);
    if (!networkingMgr->lobby->navTaken)
        guiMgr->addStaticText("NavRoleText", "(N)avigator",0.5, 0.75, 1);
}

PreGameMenu GameRoleMenu::nextMenu() {
    return PGM_NONE;
}

void GameRoleMenu::show() {
    // Show background image etc
    if (isVisible) return;

    //guiMgr->addStaticImage("GameRoleMenu",0.5, 0.5,1.0, 1.0,"GameRoleMenu","GameRoleMenu");
    isVisible = true;
}

void GameRoleMenu::hide() {
    // Hide background image etc
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("GameRoleMenu");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("PilotRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("EngRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("NavRoleText");
    CEGUI::WindowManager::getSingletonPtr()->destroyWindow("ChosenRoleText");
}

bool GameRoleMenu::end() { return isEnd; }

bool GameRoleMenu::visible() { return isVisible; }
