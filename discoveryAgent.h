#ifndef DISCOVERYAGENT_H
#define DISCOVERYAGENT_H

#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "serverInfo.h"

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "BitStream.h"

using namespace RakNet;
using namespace std;

class DiscoveryAgent {
  private:
    RakPeerInterface *server;
    RakPeerInterface *searchClient;
    bool createServer(int port);
    int numConnections;

    void handlePong(Packet *p);
    void handleServerInfo(ServerInfo *newInfo);

    bool alreadyListed(std::string address);

    std::vector<ServerInfo*> servers;
    
  public:
    DiscoveryAgent();

    void beServer(std::string gameName, bool pilotTaken, bool navTaken, bool engTaken);

    void destroyServer();

    void destroyClient();

    void startServerListUpdate(int serverPort);
    void updateServerList();
    std::vector<ServerInfo*> getServerList();
};

#endif
