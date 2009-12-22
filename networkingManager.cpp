#include "networkingManager.h"
#include <iostream>  // I/O 
#include "networkRole.h"
#include "collaborationInfo.h"
#include "gameRole.h"

const unsigned char CHOOSE_GAME_ROLE = 100;
const unsigned char START_GAME = 101;
const unsigned char GAME_ROLE_CHOICE = 102;
const unsigned char GET_ROLE_CHOICES = 103;
const unsigned char GAME_ROLE_CHOICES = 104;


using namespace std;
using namespace RakNet;

NetworkingManager::NetworkingManager(IExit *mExit) :
    serverAddress(""),
    numConnections(0),
    chosenGameRole(NO_GAME_ROLE),
    pilotTaken(false),
    navTaken(false),
    engTaken(false),
    mExit(mExit)
{
    sd.port = 0;
    replicaManager.SetDefaultPacketReliability(RELIABLE_ORDERED);
    replicaManager.SetAutoSerializeInterval(1);
}

NetworkingManager::~NetworkingManager() {
    
}

void NetworkingManager::tick() {
    bool quit = false;
    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                    quit = true;
                    break;
                case ID_DISCONNECTION_NOTIFICATION:                    printf("You have been disconnected.\n");
                    quit = true;
                    break;                case ID_CONNECTION_LOST:
                    printf("You have been disconnected.\n");
                    quit = true;
                    break;
            }
    }
    if (quit) mExit->exit();
}

NetworkRole NetworkingManager::determineRole(NetworkRole desiredRole) {
    if (desiredRole == SERVER) {
        discoveryAgent->beServer();
        return SERVER;
    }
    else if (desiredRole == CLIENT) {
        serverAddress = discoveryAgent->findServer(6001,6000,5);
        if (serverAddress.compare("") == 0) {
            printf("No servers could be found. You are now the server.\n");
            return SERVER;
        }
        return CLIENT;
    }
    else if (desiredRole == DEVELOPMENTSERVER) { return DEVELOPMENTSERVER; }
}

CollaborationInfo *NetworkingManager::startNetworking(NetworkRole desiredRole) {
    NetworkRole actualRole = NO_NETWORK_ROLE;
    discoveryAgent = new DiscoveryAgent();
    actualRole = determineRole(desiredRole);

    networkRole = actualRole;

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority((actualRole == SERVER || actualRole == DEVELOPMENTSERVER));
    
    if (actualRole == SERVER) sd.port = SERVER_PORT;

    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    if (actualRole == CLIENT) { 
        enterLobby();
    }
    else if (actualRole == SERVER) {
        initializeLobby();
        offerGameRoleChoices();
        while (pilotTaken == false || navTaken == false || engTaken == false)
        {
            discoveryAgent->beServer();
            beLobby();
        }
        
        startGame();
        discoveryAgent->destroyServer();
    }
    string nick = "rob";
    CollaborationInfo *collabInfo = new CollaborationInfo(nick, actualRole, chosenGameRole); 
    return collabInfo;
}

void NetworkingManager::startGame() {
    RakNet::BitStream dataStream;
    
    dataStream.Write(START_GAME);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkingManager::initializeLobby() {
    std::cout << "Welcome to the lobby! Just wait here while the other players connect..." << std::endl;
}

void NetworkingManager::offerGameRoleChoices() {
    if (chosenGameRole != NO_GAME_ROLE) return;
    printf("Please choose a game role from the list below:\n");
    if (!pilotTaken) printf("(P)ilot\n");
    if (!navTaken) printf("(N)avigator\n");
    if (!engTaken) printf("(E)ngineer\n");
}

void NetworkingManager::enterLobby() {
    rakPeer->Connect(serverAddress.c_str(),SERVER_PORT,0,0,0);
    bool connected = false;
    bool goodToGo = false;

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

    initializeLobby();
    requestGameRoleChoices();

    while (goodToGo != true)
    {
        unsigned char packetID;
        for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            RakNet::BitStream dataStream((unsigned char*)packet->data, packet->length, false);
            dataStream.Read(packetID);
            switch (packetID) {
                case START_GAME:
                    printf("We're good to go!\n");
                    goodToGo = true;
                    break;
                case GAME_ROLE_CHOICES:
                    dataStream.Read(pilotTaken);
                    dataStream.Read(navTaken);
                    dataStream.Read(engTaken);
                    offerGameRoleChoices();
                    break;
            }
        }
        checkForRoleChoice();
    }
}

void NetworkingManager::checkForRoleChoice() {
    if (kbhit())
    {
        char ch = getch();
        if ((ch=='p' || ch=='P') && chosenGameRole == NO_GAME_ROLE)
        {
            chosenGameRole = PILOT;
            pilotTaken = true;
            printf("You chose to be the Pilot.\n");
        }
        else if ((ch=='n' || ch=='N') && chosenGameRole == NO_GAME_ROLE)
        {
            chosenGameRole = NAVIGATOR;
            navTaken = true;
            printf("You chose to be the Navigator.\n");
        }
        else if ((ch=='e' || ch=='E') && chosenGameRole == NO_GAME_ROLE)
        {
            chosenGameRole = ENGINEER;
            engTaken = true;
            printf("You chose to be the Engineer.\n");
        }

        if (networkRole == CLIENT) {
            sendGameRoleChoice(chosenGameRole);
        }
        else if (networkRole == SERVER) sendGameRoleChoices();
    }
}

void NetworkingManager::sendGameRoleChoice(GameRole chosenGameRole) {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICE);
    if (chosenGameRole == PILOT) dataStream.Write(true); else dataStream.Write(false);
    if (chosenGameRole == NAVIGATOR) dataStream.Write(true); else dataStream.Write(false);
    if (chosenGameRole == ENGINEER) dataStream.Write(true); else dataStream.Write(false);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, rakPeer->GetSystemAddressFromIndex(0), false);
}

void NetworkingManager::requestGameRoleChoices() {
    RakNet::BitStream dataStream;

    dataStream.Write(GET_ROLE_CHOICES);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, rakPeer->GetSystemAddressFromIndex(0), false);
}

void NetworkingManager::stopNetworking() {
    rakPeer->Shutdown(100,0);
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

bool NetworkingManager::replicate(ReplicaObject *object) {
    replicaManager.Reference(object);
    return true;
}

void NetworkingManager::beLobby() {
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
                numConnections ++;
                break;
            case GET_ROLE_CHOICES:
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
    checkForRoleChoice();
}

void NetworkingManager::sendGameRoleChoices() {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICES);
    dataStream.Write(pilotTaken);
    dataStream.Write(navTaken);
    dataStream.Write(engTaken);

    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkingManager::sendGameRoleChoices(SystemAddress recipient) {
    RakNet::BitStream dataStream;

    dataStream.Write(GAME_ROLE_CHOICES);
    dataStream.Write(pilotTaken);
    dataStream.Write(navTaken);
    dataStream.Write(engTaken);

    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, recipient, false);
}

ReplicaObject* NetworkingManager::getReplica(string name, bool blocking) {
    DataStructures::Multilist<ML_STACK, Replica3*> replicaList; 
    DataStructures::DefaultIndexType index;
    replicaManager.GetReferencedReplicaList(replicaList);

    while (true) {
        try {
            for (index=0; index < replicaList.GetSize(); index++) {
                ReplicaObject * temp = ((ReplicaObject *) replicaList[index]);
                if (temp->GetName().StrCmp(RakNet::RakString(name.c_str())) == 0) return temp;
            }
        }
        catch (...) {

        }
        if (!blocking) return 0;
        this->tick();
        replicaManager.GetReferencedReplicaList(replicaList);
    }
}

ReplicaObject* NetworkingManager::getReplica(int index, bool blocking) {
    if (blocking) {
        while (replicaManager.GetReplicaCount() == 0) {
            this->tick();
        }
    }
    try {
        return (ReplicaObject *) replicaManager.GetReplicaAtIndex(index);
    }
    catch (...) {
        return 0;
    }
}

