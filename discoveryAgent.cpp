
#include "discoveryAgent.h"

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

using namespace std;

DiscoveryAgent::DiscoveryAgent() :
    server(0)
{
    searchClient=RakNetworkFactory::GetRakPeerInterface();

    SocketDescriptor socketDescriptor(6000,0);
    searchClient->Startup(1, 30, &socketDescriptor, 1);
}

void DiscoveryAgent::startServerListUpdate(int serverPort) {

    try {
        searchClient->Ping("255.255.255.255",serverPort,true);
    } catch (...) {
        cout << "Could not start ping - program already running?\n";
        exit(-1);
    }

    for(std::vector<ServerInfo*>::const_iterator ite=servers.begin();ite!=servers.end();++ite) {
        ServerInfo *info = *ite;
        info->setLastPonged(info->getLastPong() + 1);
    }
}

void DiscoveryAgent::updateServerList() {

    Packet *p = searchClient->Receive();
    if (p==0) {
        return;
    }
    else {
        if (p->data[0]==ID_PONG) {
            handlePong(p);
        }
        searchClient->DeallocatePacket(p);
    }

}

void DiscoveryAgent::handlePong(Packet *p) {
    string temp = p->systemAddress.ToString();
    string serverIP = temp.substr(0, temp.find(":"));

    bool pilotTaken = false;
    bool engTaken = false;
    bool navTaken = false;

    RakNet::BitStream bs(p->data, p->length, false);
    bs.IgnoreBytes(1);
    RakNet::RakString rakgamename;
    RakNetTime time;
    bs.Read(time);
    bs.Read(rakgamename);
    bs.Read(pilotTaken);
    bs.Read(navTaken);
    bs.Read(engTaken);

    int latency = (int)(RakNet::GetTime()-time);
    string gameName = rakgamename.C_String();
    ServerInfo *serverInfo = new ServerInfo(serverIP, gameName,
                                            pilotTaken,navTaken,engTaken,latency);
    
    handleServerInfo(serverInfo);
}

void DiscoveryAgent::handleServerInfo(ServerInfo *newInfo) {

    if (newInfo->getName().compare("") == 0)
        return; // Invalid game

    for(std::vector<ServerInfo*>::const_iterator ite=servers.begin();ite!=servers.end();++ite) {
        ServerInfo *oldInfo = *ite;
        if (oldInfo->getAddress().compare(newInfo->getAddress()) == 0) {
            //Update existing
            oldInfo->setPilotTaken(newInfo->isPilotTaken());
            oldInfo->setNavigatorTaken(newInfo->isNavigatorTaken());
            oldInfo->setEngineerTaken(newInfo->isEngineerTaken());
            oldInfo->setLastPonged(0);
            oldInfo->setLatency(newInfo->getLatency());
            delete newInfo;
            return;
        }
    }

    // Server is not in the list, so add it
    servers.push_back(newInfo);
}

bool DiscoveryAgent::alreadyListed(std::string address) {
    for(std::vector<ServerInfo*>::const_iterator ite=servers.begin();ite!=servers.end();++ite) {
        if ((*ite)->getAddress().compare(address) == 0) return true;
    }
    return false;
}

std::vector<ServerInfo*> DiscoveryAgent::getServerList() { return servers; }

void DiscoveryAgent::beServer(string gameName, bool pilotTaken, bool navTaken, bool engTaken) {
    if (server == 0) createServer(6001);
    Packet *p;

    p = server->Receive();
    if (p==0) {
        SLEEP(30);
    }
    else {
        RakNet::BitStream dataStream;
        dataStream.Write(RakNet::RakString(gameName.c_str()));
        dataStream.Write(pilotTaken);
        dataStream.Write(navTaken);
        dataStream.Write(engTaken);

        server->SetOfflinePingResponse((const char*) dataStream.GetData(),dataStream.GetNumberOfBytesUsed());
        server->DeallocatePacket(p);
        std::cout << "Ponging with name " << gameName << " and roles " << pilotTaken << " " << navTaken << " " << engTaken << "\n";
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

void DiscoveryAgent::destroyClient() {
    RakNetworkFactory::DestroyRakPeerInterface(searchClient);
}

