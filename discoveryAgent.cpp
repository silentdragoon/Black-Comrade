#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "BitStream.h"
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "discoveryAgent.h"

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

DiscoveryAgent::DiscoveryAgent() :
    server(0)
{
    searchClient=RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(6000,0);
    searchClient->Startup(1, 30, &socketDescriptor, 1);
}

using namespace std;

void DiscoveryAgent::startServerListUpdate(int serverPort) {
    std::cout << "Pinging..." << std::endl;
    searchClient->Ping("255.255.255.255",serverPort,true);
}

void DiscoveryAgent::updateServerList() {

    Packet *p = searchClient->Receive();
    if (p==0) {
        return;
    }
    else {
       if (p->data[0]==ID_PONG) {
            string temp = p->systemAddress.ToString();
            string serverIP = temp.substr(0, temp.find(":"));
            servers.push_back(serverIP);
            // TODO: Only add to the list if the server does not already exist in it
        }
        searchClient->DeallocatePacket(p);
    }

}

std::vector<string> DiscoveryAgent::getServerList() { return servers; }

void DiscoveryAgent::beServer() {
    if (server == 0) createServer(6001);
    Packet *p;

    p = server->Receive();
    if (p==0)
    {
        SLEEP(30);
    }
    else
    {
        server->DeallocatePacket(p);
    }
}

bool DiscoveryAgent::createServer(int port) {
    server = RakNetworkFactory::GetRakPeerInterface();
    bool b;
    SocketDescriptor socketDescriptor(port,0);
    b = server->Startup(2, 30, &socketDescriptor, 1);
    server->SetMaximumIncomingConnections(3);
    if (!b)
    {
        printf("Server failed to start. Terminating.\n");
        exit(1);
    }
    return b;
}

void DiscoveryAgent::destroyServer() {
    RakNetworkFactory::DestroyRakPeerInterface(server);
}

