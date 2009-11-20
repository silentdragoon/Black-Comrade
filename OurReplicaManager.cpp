#include "OurReplicaManager.h"	

#include "ReplicaManager3.h"
#include "ReplicaConnection.h"

using namespace RakNet;

Connection_RM3* OurReplicaManager::AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const {
	return new ReplicaConnection(systemAddress,rakNetGUID);
}

void OurReplicaManager::DeallocConnection(Connection_RM3 *connection) const {
	delete connection;
}

Replica3* OurReplicaManager::GetShipState() {
    if (GetReplicaCount() > 0) {
        return GetReplicaAtIndex(0);
    }
    else
    {
        return 0;
    }
}

