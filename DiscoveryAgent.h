#include <string>
using namespace std;

class DiscoveryAgent {
  public:
    DiscoveryAgent();
    string findServer(int serverPort, int clientPort, int timeout);
    void beServer(int serverPort, int timeout);
};
