#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include "ITickable.h"

// raknet required includes
#include "StringTable.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include <stdio.h>
#include "Kbhit.h"
#include <string.h>
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "RakSleep.h"
#include "FormatString.h"
#include "RakString.h"
#include "GetTime.h"
#include "SocketLayer.h"

#include "DiscoveryAgent.h"

#include "ReplicaObject.h"
#include "shipState.h"
#include "SystemsState.h"
#include "ReplicaConnection.h"
#include "OurReplicaManager.h"

#include <string>
#include <cstdio>

using namespace std;
using namespace RakNet;

class NetworkingManager : public ITickable {

private:
        DiscoveryAgent *discoveryAgent;
        ShipState *shipState;
        SystemsState *systemsState;
        Packet *packet;
        SocketDescriptor sd;
        static const int SERVER_PORT=6001;
        NetworkIDManager networkIdManager;
        bool isServer;


        RakPeerInterface *rakPeer;


        string serverAddress;

public:

        OurReplicaManager replicaManager;
        NetworkingManager();

        virtual void tick();
        void startNetworking(bool Server);

};


#endif
