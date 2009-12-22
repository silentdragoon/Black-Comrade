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

