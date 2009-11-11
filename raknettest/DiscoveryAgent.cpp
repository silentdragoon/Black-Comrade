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

char* DiscoveryAgent::findServer(int serverPort, int clientPort, int timeout)
{
    RakPeerInterface *client;
    Packet *p;
    char server[22] = "";
    client=RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(clientPort,0);
    client->Startup(1, 30, &socketDescriptor, 1);

    client->Ping("255.255.255.255", serverPort, true);

    printf("Pinging...\n");

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
                printf("Got pong from %s with time %i\n", p->systemAddress.ToString(), RakNet::GetTime() - time);
                strcpy(server,p->systemAddress.ToString());
                break;
            }
            else
            {
                printf("%s\n", p->data);
            }
            client->DeallocatePacket(p);
        }

        SLEEP(30);
    }
    RakNetworkFactory::DestroyRakPeerInterface(client);
    return strdup(server);
}

void DiscoveryAgent::beServer(int serverPort, int timeout)
{
    RakPeerInterface *server;
    bool b;

    int numConnections = 0;

    Packet *p;

    server = RakNetworkFactory::GetRakPeerInterface();
    printf("Starting server.\n");

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

    RakNetTime quitTime = RakNet::GetTime() + (timeout * 1000);

    while (RakNet::GetTime() < quitTime)
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
            if (numConnections == 2)
            {
              printf("Two pings received. Continuing.\n");
              break;
            }
        }
    }
    RakNetworkFactory::DestroyRakPeerInterface(server);
}
