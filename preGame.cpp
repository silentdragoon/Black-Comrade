#include "preGame.h"

PreGame::PreGame(InputState *inputState, NetworkingManager *networkingManager)
    : inputState(inputState)
    , networkingManager(networkingManager)
{}

CollaborationInfo* PreGame::run() {
    CollaborationInfo *collabInfo;

    std::cout << "***Input is now capured by the main window!***\n"; 
    std::cout << "Pick network role [ D | S | C ]\n";
    while(true) {
        NetworkRole desiredRole = NO_NETWORK_ROLE;
        if (inputState->isKeyDown(OIS::KC_D)) {
            desiredRole = DEVELOPMENTSERVER;
        } else if (inputState->isKeyDown(OIS::KC_S)) {
            desiredRole = SERVER;
        } else if (inputState->isKeyDown(OIS::KC_C))	 {
            desiredRole = CLIENT;
        }
        if (desiredRole != NO_NETWORK_ROLE) {
            if (!networkingManager->startNetworking(desiredRole)) {
                std::cout << "Could not start networking." << std::endl;
                std::exit(-1);
            } else break;
        }
        inputState->tick();
    }


    networkingManager->lobby->chooseNick("Player");

    std::cout << "Pick game role [ P | E | N ]\n";
    while (true) {
        collabInfo = runLobby();
        if (collabInfo->getGameRole() != NO_GAME_ROLE) break;
        if (inputState->isKeyDown(OIS::KC_P)) {
            networkingManager->lobby->chooseGameRole(PILOT);
        } else if (inputState->isKeyDown(OIS::KC_N)) {
            networkingManager->lobby->chooseGameRole(NAVIGATOR);
        } else if (inputState->isKeyDown(OIS::KC_E))	 {
            networkingManager->lobby->chooseGameRole(ENGINEER);
        }
        inputState->tick();
    }
    return collabInfo;
}

CollaborationInfo *PreGame::runLobby() {
    networkingManager->tick();
    return networkingManager->collabInfo;
}
