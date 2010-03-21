#include "lobby.h"
#include "Kbhit.h"

using namespace std;
using namespace RakNet;

Lobby::Lobby(RakPeerInterface * rp, DiscoveryAgent *da, NetworkRole nr)
    : pilotTaken(false)
    , navTaken(false)
    , engTaken(false)
    , roleOptionsChanged(true)
{
    rakPeer = rp;
    networkRole = nr;
    discoveryAgent = da;
    gameRole = NO_GAME_ROLE;
    chosenGameRole = NO_GAME_ROLE;
}

void Lobby::enter() {
    if (networkRole == DEVELOPMENTSERVER) {
        nick = "Player";
        return;
    }
    std::cout << "Welcome to the lobby" << std::endl;
    if (networkRole == SERVER) offerGameRoleChoices();
}

GameRole Lobby::getChosenGameRole() { return gameRole; }

bool Lobby::hasChosenRole() { return (gameRole != NO_GAME_ROLE); }

string Lobby::getChosenNick() { return nick; }

void Lobby::offerGameRoleChoices() {
    roleOptionsChanged = true;
    if (gameRole != NO_GAME_ROLE) return;
    printf("Available roles:\n");
    if (!pilotTaken) printf("(P)ilot\n");
    if (!navTaken) printf("(N)avigator\n");
    if (!engTaken) printf("(E)ngineer\n");
}

bool Lobby::connect(string serverAddress, int port) {
    rakPeer->Connect(serverAddress.c_str(),port,0,0,0);
    bool connected = false;

    while(connected != true) {
        for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    connected = true;
                    break;
            }
        }
    }
    return connected;
}

void Lobby::chooseNick(string mNick) {
    nick = mNick;
}

void Lobby::chooseGameRole(GameRole role) {
    chosenGameRole = role;
}

bool Lobby::wait() {
    roleOptionsChanged = false;
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
    if (chosenGameRole == NO_GAME_ROLE) return;

    if (chosenGameRole == PILOT && gameRole == NO_GAME_ROLE && pilotTaken == false)
    {
        gameRole = PILOT;
        pilotTaken = true;
        printf("You chose to be the Pilot.\n");
        roleOptionsChanged = true;
    }
    else if (chosenGameRole == NAVIGATOR && gameRole == NO_GAME_ROLE && navTaken == false)
    {
        gameRole = NAVIGATOR;
        navTaken = true;
        printf("You chose to be the Navigator.\n");
        roleOptionsChanged = true;
    }
    else if (chosenGameRole == ENGINEER && gameRole == NO_GAME_ROLE && engTaken == false)
    {
        gameRole = ENGINEER;
        engTaken = true;
        printf("You chose to be the Engineer.\n");
        roleOptionsChanged = true;
    }

    if (networkRole == CLIENT) {
        sendGameRoleChoice(gameRole);
    }
    else if (networkRole == SERVER) sendGameRoleChoices();

    chosenGameRole = NO_GAME_ROLE;
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
            case ID_PING:
                sendGameInfo(packet->systemAddress);
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

void Lobby::sendGameInfo(SystemAddress recipient) {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_INFO);
    dataStream.Write(pilotTaken);
    dataStream.Write(navTaken);
    dataStream.Write(engTaken);

    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, recipient, false);
}
