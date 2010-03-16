#ifndef DISCOVERYAGENT_H
#define DISCOVERYAGENT_H

#include <string>
#include <vector>

#include "RakPeerInterface.h"

using namespace RakNet;
using namespace std;

class DiscoveryAgent {
  private:
    RakPeerInterface *server;
    bool createServer(int port);
    int numConnections;
    
  public:
    DiscoveryAgent();
    string findServer(int serverPort, int clientPort, int timeout);
    std::vector<string> findServers(int serverPort, int clientPort, int timeout);
    void beServer();
    void destroyServer();
};

#endif
