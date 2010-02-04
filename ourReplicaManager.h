#ifndef OURREPLICAMANAGER_H
#define OURREPLICAMANAGER_H

#include "ReplicaManager3.h"
#include "RakNetTime.h"
#include "replicaConnection.h"

using namespace RakNet;

class OurReplicaManager : public ReplicaManager3
{
	virtual Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const;
	virtual void DeallocConnection(Connection_RM3 *connection) const;

public:
       Replica3 *GetShipState();
       void doUpdate();
};

#endif
