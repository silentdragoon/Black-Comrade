#include <string>
using namespace std;

class DiscoveryAgent {
  public:
    DiscoveryAgent();
    char* findServer(int serverPort, int clientPort, int timeout);
    void beServer(int serverPort, int timeout);
};
