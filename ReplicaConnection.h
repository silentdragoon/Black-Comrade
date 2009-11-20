#ifndef REPLICACONNECTION_H
#define REPLICACONNECTION_H

#include "ReplicaManager3.h"
#include "shipState.h"

using namespace RakNet;

class ReplicaConnection : public Connection_RM3 {

public:
    ReplicaConnection(SystemAddress _systemAddress, RakNetGUID _guid);
    virtual ~ReplicaConnection();
    virtual Replica3 *AllocReplica(RakNet::BitStream *allocationId, ReplicaManager3 *replicaManager3);

protected:
};

#endif
