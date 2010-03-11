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
{
    CEGUI::ImagesetManager::getSingleton().create("NetworkRoleMenu.imageset");
}

void NetworkRoleMenu::tick() {
    // Check for key presses etc
    NetworkRole desiredRole = NO_NETWORK_ROLE;

    if (inputState->isKeyDown(OIS::KC_D)) {
        desiredRole = DEVELOPMENTSERVER;
    } else if (inputState->isKeyDown(OIS::KC_S)) {
        desiredRole = SERVER;
    } else if (inputState->isKeyDown(OIS::KC_C))	 {
        desiredRole = CLIENT;
    } else if (inputState->isKeyDown(OIS::KC_ESCAPE)) {
        std::exit(-1);
    }
    if (desiredRole != NO_NETWORK_ROLE) {
        if (!networkingMgr->startNetworking(desiredRole)) {
            std::cout << "Could not start networking." << std::endl;
            std::exit(0);
        } else isEnd = true;
    }
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
