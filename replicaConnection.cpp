#include "replicaConnection.h"
#include "ourReplicaManager.h"

#include "ReplicaManager3.h"

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
    else if (typeName=="PilotGunState" || typeName == "EngineerGunState" || typeName == "NavigatorGunState") {
        return new GunState;
    }
    else if (typeName=="GameStateMachine") {
        return new GameStateMachine;
    }
    else if (typeName=="Enemy") {
        return new Enemy;
    }
    else if (typeName=="DamageState") {
        return new DamageState;
    }
    else if (typeName=="NotificationManager") {
        return new NotificationManager;
    }
    ((OurReplicaManager *) replicaManager3)->doUpdate();
    return 0;
}

DataStructures::Multilist<ML_STACK, LastSerializationResult*, Replica3*> ReplicaConnection::getList() {
    return queryToSerializeReplicaList;
}

