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
    server(0) {}

using namespace std;

string DiscoveryAgent::findServer(int serverPort, int clientPort, int timeout)
{
    RakPeerInterface *client;
    Packet *p;
    string serverIP = "";
    client=RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(clientPort,0);
    client->Startup(1, 30, &socketDescriptor, 1);

    client->Ping("255.255.255.255", serverPort, true);

    printf("Looking for servers...\n");

    RakNetTime quitTime = RakNet::GetTime() + (timeout * 1000);

    while (RakNet::GetTime() < quitTime)
    {
        p = client->Receive();
        if (p==0)
        {
            SLEEP(30);
            continue;
        }
        else
        {
            if (p->data[0]==ID_PONG)
            {
                RakNetTime time;
                memcpy((char*)&time, p->data+1, sizeof(RakNetTime));
                string temp = p->systemAddress.ToString();
                serverIP = temp.substr(0, temp.find(":"));
                break;
            }
            client->DeallocatePacket(p);
        }
        SLEEP(30);
    }

    RakNetworkFactory::DestroyRakPeerInterface(client);
    return serverIP;
}

std::vector<string> DiscoveryAgent::findServers(int serverPort, int clientPort, int timeout) {
    std::vector<string> servers;

    RakPeerInterface *client;
    Packet *p;
    string serverIP = "";
    client=RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(clientPort,0);
    client->Startup(1, 30, &socketDescriptor, 1);

    client->Ping("255.255.255.255", serverPort, true);

    printf("Looking for servers...\n");

    RakNetTime quitTime = RakNet::GetTime() + (timeout * 1000);

    while (RakNet::GetTime() < quitTime)
    {
        p = client->Receive();
        if (p==0)
        {
            SLEEP(30);
            continue;
        }
        else
        {
            if (p->data[0]==ID_PONG)
            {
                RakNetTime time;
                memcpy((char*)&time, p->data+1, sizeof(RakNetTime));
                string temp = p->systemAddress.ToString();
                serverIP = temp.substr(0, temp.find(":"));
                servers.push_back(serverIP);
            }
            client->DeallocatePacket(p);
        }
        SLEEP(30);
    }

    RakNetworkFactory::DestroyRakPeerInterface(client);
    return servers;
}

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

