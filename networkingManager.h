#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include "ITickable.h"
#include "IExit.h"

// raknet required includes

#include "replicaObject.h"
#include "shipState.h"
#include "replicaConnection.h"
#include "ourReplicaManager.h"
#include "collaborationInfo.h"
#include "discoveryAgent.h"
#include "lobby.h"

#include "gameRole.h"

#include <vector>
#include <string>
#include <cstdio>

class NetworkingManager : public ITickable {

private:

        GameRole chosenGameRole;
        NetworkRole networkRole;
        Packet *packet;
        DiscoveryAgent *discoveryAgent;

        SocketDescriptor sd;

        NetworkIDManager networkIdManager;
        bool isServer;
        RakPeerInterface *rakPeer;
        string serverAddress;
        NetworkRole determineRole(NetworkRole desiredRole);

        IExit *mExit;

        bool inLobby;
        void startGame();
        int numConnections;


public:
        Lobby *lobby;

        CollaborationInfo *collabInfo;
        NetworkingManager(IExit *mExit);
        ~NetworkingManager();

        OurReplicaManager replicaManager;
        virtual void tick();

        bool startNetworking(NetworkRole desiredRole);

        void runLobby();

        void stopNetworking();

        bool replicate(ReplicaObject *object);
        ReplicaObject *getReplica(string name, bool blocking);
        ReplicaObject *getReplica(int index, bool blocking);
        std::vector<ReplicaObject*> getReplicas(string name);
};


#endif
