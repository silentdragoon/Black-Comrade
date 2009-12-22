#include "networkingManager.h"
#include <iostream>  // I/O 
#include "networkRole.h"

using namespace std;
using namespace RakNet;

NetworkingManager::NetworkingManager(IExit *mExit) :
    serverAddress(""),
    mExit(mExit)
{}

NetworkingManager::~NetworkingManager() {
    
}

void NetworkingManager::tick() {
    bool quit = false;
    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    //printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
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

NetworkRole NetworkingManager::startNetworking(NetworkRole desiredRole) {
    NetworkRole actualRole = NONE;
    discoveryAgent = new DiscoveryAgent();
    if (desiredRole == SERVER) {
        discoveryAgent->beServer(6001,1);
        actualRole = SERVER;
    }
    else if (desiredRole == CLIENT) {
        serverAddress = discoveryAgent->findServer(6001,6000,5);
        if (serverAddress.compare("") == 0) {
            printf("No servers could be found. You are now the server.\n");
            return this->startNetworking(SERVER);
        }
        actualRole = CLIENT;
    }
    else if (desiredRole == DEVELOPMENTSERVER) { actualRole = DEVELOPMENTSERVER; }

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority((actualRole == SERVER || actualRole == DEVELOPMENTSERVER));
    replicaManager.SetDefaultPacketReliability(RELIABLE_ORDERED);
    replicaManager.SetAutoSerializeInterval(1);

    if (actualRole == SERVER) {
        sd.port=SERVER_PORT;
    }
    else {
        sd.port=0;
    }
    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    bool connected = false;
    int numConnections = 0;
    if (actualRole == CLIENT) {
        rakPeer->Connect(serverAddress.c_str(),SERVER_PORT,0,0,0);
    }
    else if (actualRole == DEVELOPMENTSERVER) return actualRole;

    while(!connected) {
        for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    //printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                    break;
                case ID_NEW_INCOMING_CONNECTION:
                    //printf("ID_NEW_INCOMING_CONNECTION from %s\n", packet->systemAddress.ToString());
                    connected = true;
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    //printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
                    connected = true;
                    break;
            }
        }
    }
    return actualRole;
}

void NetworkingManager::stopNetworking() {
    rakPeer->Shutdown(100,0);
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

bool NetworkingManager::replicate(ReplicaObject *object) {
    replicaManager.Reference(object);
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

