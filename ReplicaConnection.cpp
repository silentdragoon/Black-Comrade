#include "ReplicaConnection.h"

#include "ReplicaManager3.h"
#include "shipState.h"
#include "frontGunState.h"

using namespace RakNet;

ReplicaConnection::ReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid) : Connection_RM3(_systemAddress, _guid) {}

ReplicaConnection::~ReplicaConnection() {}

Replica3 *ReplicaConnection::AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3)
{
    RakNet::RakString typeName;
    allocationId->Read(typeName);
    if (typeName=="ShipState") {
        return new ShipState;
    }
    else if (typeName=="FrontGunState") {
        return new FrontGunState;
    }
    return 0;
}

