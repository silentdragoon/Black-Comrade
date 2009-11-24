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
#include "DiscoveryAgent.h"

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

DiscoveryAgent::DiscoveryAgent(){}

using namespace std;

string DiscoveryAgent::findServer(int serverPort, int clientPort, int timeout)
{
    RakPeerInterface *client;
    Packet *p;
    string server = "";
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
                //printf("Got pong from %s with time %i\n", p->systemAddress.ToString(), RakNet::GetTime() - time);
		string temp = p->systemAddress.ToString();
                server = temp.substr(0, temp.find(":"));
                break;
            }
            client->DeallocatePacket(p);
        }
        SLEEP(30);
    }
    printf("Finished looking\n");
    RakNetworkFactory::DestroyRakPeerInterface(client);
    return server;
}

void DiscoveryAgent::beServer(int serverPort, int numClients)
{
    RakPeerInterface *server;
    bool b;

    int numConnections = 0;

    Packet *p;

    server = RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(serverPort,0);
    b = server->Startup(2, 30, &socketDescriptor, 1);
    server->SetMaximumIncomingConnections(2);
    if (b)
        printf("Server started, waiting for connections.\n");
    else
    {
        printf("Server failed to start. Terminating.\n");
        exit(1);
    }

    while (true)
    {
        p = server->Receive();
        if (p==0)
        {
            SLEEP(30);
            continue;
        }
        else
        {
            server->DeallocatePacket(p);
            numConnections = numConnections + 1;
            if (numConnections == numClients)
            {
              break;
            }
        }
    }
    RakNetworkFactory::DestroyRakPeerInterface(server);
}
