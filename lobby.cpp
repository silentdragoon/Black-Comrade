#include "lobby.h"
#include "Kbhit.h"

using namespace std;
using namespace RakNet;

Lobby::Lobby(RakPeerInterface * rp, DiscoveryAgent *da, NetworkRole nr)
{
    rakPeer = rp;
    networkRole = nr;
    discoveryAgent = da;
    gameRole = NO_GAME_ROLE;
}

void Lobby::enter() {
    if (networkRole == DEVELOPMENTSERVER) {
        nick = "Player";
        return;
    }
    std::cout << "Please type a nickname:" << std::endl;
    getline(cin, nick);
    std::cout << "Welcome to the lobby, " << nick << "! Just wait here while the other players connect..." << std::endl;
    if (networkRole == SERVER) offerGameRoleChoices();
}

GameRole Lobby::getChosenGameRole() { return gameRole; }

string Lobby::getChosenNick() { return nick; }

void Lobby::offerGameRoleChoices() {
    if (gameRole != NO_GAME_ROLE) return;
    printf("Please choose a game role from the list below:\n");
    if (!pilotTaken) printf("(P)ilot\n");
    if (!navTaken) printf("(N)avigator\n");
    if (!engTaken) printf("(E)ngineer\n");
}

void Lobby::connect(string serverAddress, int port) {
    rakPeer->Connect(serverAddress.c_str(),port,0,0,0);
    bool connected = false;

    while(connected != true) {
        for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    exit(1);
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    connected = true;
                    break;
            }
        }
    }
}

bool Lobby::wait() {
    if (networkRole == CLIENT) {
            unsigned char packetID;
            for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
                RakNet::BitStream dataStream((unsigned char*)packet->data, packet->length, false);
                dataStream.Read(packetID);
                switch (packetID) {
                    case START_GAME:
                        printf("We're good to go!\n");
                        return true;
                        break;
                    case GAME_ROLE_CHOICES:
                        dataStream.Read(pilotTaken);
                        dataStream.Read(navTaken);
                        dataStream.Read(engTaken);
                        offerGameRoleChoices();
                        break;
                }
            }
    }
    else if (networkRole == SERVER) {
        discoveryAgent->beServer();
        process();
        if (pilotTaken && navTaken && engTaken) return true;
    }
    else if (networkRole == DEVELOPMENTSERVER) {
        gameRole = PILOT;
        return true;
    }
    checkForRoleChoice();
    return false;
}

void Lobby::checkForRoleChoice() {
    if (kbhit())
    {
        char ch = getch();
        if ((ch=='p' || ch=='P') && gameRole == NO_GAME_ROLE)
        {
            gameRole = PILOT;
            pilotTaken = true;
            printf("You chose to be the Pilot.\n");
        }
        else if ((ch=='n' || ch=='N') && gameRole == NO_GAME_ROLE)
        {
            gameRole = NAVIGATOR;
            navTaken = true;
            printf("You chose to be the Navigator.\n");
        }
        else if ((ch=='e' || ch=='E') && gameRole == NO_GAME_ROLE)
        {
            gameRole = ENGINEER;
            engTaken = true;
            printf("You chose to be the Engineer.\n");
        }

        if (networkRole == CLIENT) {
            sendGameRoleChoice(gameRole);
        }
        else if (networkRole == SERVER) sendGameRoleChoices();
    }
}

void Lobby::process() {
    unsigned char packetID;
    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
        RakNet::BitStream dataStream((unsigned char*)packet->data, packet->length, false);
        dataStream.Read(packetID);
        switch (packetID) {
            case ID_CONNECTION_ATTEMPT_FAILED:
                exit(1);
                break;
            case ID_NEW_INCOMING_CONNECTION:
                printf("A new player has connected.\n");
                sendGameRoleChoices(packet->systemAddress);
                break;
            case GAME_ROLE_CHOICE:
                bool someoneChosePilot;
                bool someoneChoseNav;
                bool someoneChoseEng;
                dataStream.Read(someoneChosePilot);
                dataStream.Read(someoneChoseNav);
                dataStream.Read(someoneChoseEng);
                pilotTaken = pilotTaken || someoneChosePilot;
                navTaken = navTaken || someoneChoseNav;
                engTaken = engTaken || someoneChoseEng;
                sendGameRoleChoices();
                offerGameRoleChoices();
                break;
        }
    }
}

void Lobby::sendGameRoleChoices() {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICES);
    dataStream.Write(pilotTaken);
    dataStream.Write(navTaken);
    dataStream.Write(engTaken);

    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Lobby::sendGameRoleChoices(SystemAddress recipient) {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICES);
    dataStream.Write(pilotTaken);
    dataStream.Write(navTaken);
    dataStream.Write(engTaken);

    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, recipient, false);
}

void Lobby::sendGameRoleChoice(GameRole chosenGameRole) {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICE);
    if (chosenGameRole == PILOT) dataStream.Write(true); else dataStream.Write(false);
    if (chosenGameRole == NAVIGATOR) dataStream.Write(true); else dataStream.Write(false);
    if (chosenGameRole == ENGINEER) dataStream.Write(true); else dataStream.Write(false);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, rakPeer->GetSystemAddressFromIndex(0), false);
}
