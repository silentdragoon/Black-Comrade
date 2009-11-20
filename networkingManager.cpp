#include "networkingManager.h"
#include <iostream>  // I/O 


using namespace std;
using namespace RakNet;

NetworkingManager::NetworkingManager() :
    serverAddress("")
{}

void NetworkingManager::tick() {
    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                    break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
                    break;
            }
    }
}

void NetworkingManager::startNetworking(bool beServer) {
    discoveryAgent = new DiscoveryAgent();
    if (beServer) {
        discoveryAgent->beServer(6001,5);
    }
    else {
        serverAddress = discoveryAgent->findServer(6001,6000,5);
        if (serverAddress.compare("") != 0) cout << serverAddress << std::endl;
    }

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority(beServer);
    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    bool connected = false;
    if (!beServer) {
        rakPeer->Connect(serverAddress.c_str(),SERVER_PORT,0,0,0);
        while(!connected) {
            for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
                switch (packet->data[0]) {
                    case ID_CONNECTION_ATTEMPT_FAILED:
                        printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                        break;
                    case ID_CONNECTION_REQUEST_ACCEPTED:
                        printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
                        connected = true;
                        break;
                }
            }
            RakSleep(30);
        }
    }
}

