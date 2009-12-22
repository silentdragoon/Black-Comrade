#include <string>
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
    void beServer();
    void destroyServer();
};
