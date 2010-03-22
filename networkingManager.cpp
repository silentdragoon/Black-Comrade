#include "networkingManager.h"
#include <iostream>  // I/O 
#include "networkRole.h"
#include "collaborationInfo.h"
#include "gameRole.h"
#include <vector>
#include <stdio.h>
#include "lobby.h"
#include "const.h"

using namespace std;
using namespace RakNet;

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

NetworkingManager::NetworkingManager(IExit *mExit) :
    serverAddress(""),
    numConnections(0),
    chosenGameRole(NO_GAME_ROLE),
    mExit(mExit),
    inLobby(true)
{
    sd.port = 0;
    replicaManager.SetDefaultPacketReliability(RELIABLE_ORDERED);
    replicaManager.SetAutoSerializeInterval(1);

    collabInfo = new CollaborationInfo("",NO_NETWORK_ROLE,NO_GAME_ROLE);
    discoveryAgent = new DiscoveryAgent();
}

NetworkingManager::~NetworkingManager() {
    cout << "...packet" << endl;
    delete packet;
    cout << "...discoveryagent" << endl;
    delete discoveryAgent;
    cout << "...lobby" << endl;
    delete lobby;
}

void NetworkingManager::tick() {
    bool quit = false;

    if (inLobby) {
        runLobby();
        return;
    }

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
                case END_GAME:
                    printf("Ending the game...\n");
                    quit = true;
                    break;
            }
    }
    if (quit) mExit->exit();
    replicaManager.doUpdate();
}

bool NetworkingManager::hostGame(bool development) {
    NetworkRole actualRole = NO_NETWORK_ROLE;

    if (development)
        networkRole = DEVELOPMENTSERVER;
    else
        networkRole = SERVER;

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority(true);
    
    if (networkRole == SERVER) sd.port = Const::SERVER_PORT;

    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    lobby = new Lobby(rakPeer, discoveryAgent, networkRole);
    return true;
}

bool NetworkingManager::connectToGame(int id) {
    if (discoveryAgent->getServerList().size() == 0) return false;

    std::cout << "About to connect to " << discoveryAgent->getServerList().at(0) << "\n";

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority(false);

    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    lobby = new Lobby(rakPeer, discoveryAgent, CLIENT);
    return lobby->connect(discoveryAgent->getServerList().at(0), Const::SERVER_PORT);
}

void NetworkingManager::runLobby() {
    if (!lobby->wait()) return;
    
    if (networkRole == SERVER) {
        startGame();
        discoveryAgent->destroyServer();
    }
    chosenGameRole = lobby->getChosenGameRole();
    if (chosenGameRole == NO_GAME_ROLE) chosenGameRole = PILOT;
    collabInfo = new CollaborationInfo(lobby->getChosenNick(), networkRole, chosenGameRole);
    inLobby = false;
}

void NetworkingManager::startGame() {
    RakNet::BitStream dataStream;
    
    dataStream.Write(START_GAME);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkingManager::endGame() {
    RakNet::BitStream dataStream;
    
    dataStream.Write(END_GAME);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkingManager::stopNetworking() {
    rakPeer->Shutdown(100,0);
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

bool NetworkingManager::replicate(ReplicaObject *object) {
    replicaManager.Reference(object);
    replicaManager.doUpdate();
    return true;
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

std::vector<ReplicaObject*> NetworkingManager::getReplicas(string name) {
    DataStructures::Multilist<ML_STACK, Replica3*> replicaList;
    DataStructures::DefaultIndexType index;
    replicaManager.GetReferencedReplicaList(replicaList);

    std::vector<ReplicaObject*> replicas = std::vector<ReplicaObject*>();
    try {
        for (index=0; index < replicaList.GetSize(); index++) {
            ReplicaObject * temp = ((ReplicaObject *) replicaList[index]);
            if (temp->GetName().StrCmp(RakNet::RakString(name.c_str())) == 0) replicas.push_back(temp);
        }
    }
    catch (...) {}
    return replicas;
}

