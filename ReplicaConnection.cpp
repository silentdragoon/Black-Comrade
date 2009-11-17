#include "ReplicaConnection.h"

#include "ReplicaManager3.h"
#include "shipState.h"
#include "SystemsState.h"

using namespace RakNet;

ReplicaConnection::ReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) {}

ReplicaConnection::~ReplicaConnection() {}

Replica3 *ReplicaConnection::AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3)
{
		RakNet::RakString typeName;
		allocationId->Read(typeName);
		if (typeName=="ShipState")
		{
			return new ShipState;
		}
		if (typeName=="SystemsState")
		{
			return new SystemsState;
		}
		return 0;
}
