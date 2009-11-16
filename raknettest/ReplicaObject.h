#ifndef REPLICAOBJECT_H
#define REPLICAOBJECT_H

#include "ReplicaManager3.h"

using namespace RakNet;

class ReplicaObject : public Replica3
{
public:

	virtual RakNet::RakString GetName(void) const=0;

	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const;

	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);

	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);

	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection);

	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection);

	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection);

	virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

	virtual void SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection);

	virtual void DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection);

	virtual void SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection);

	virtual void DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection);

	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3);

	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection);

	virtual RM3QuerySerializationResult QuerySerialization(RakNet::Connection_RM3 *destinationConnection);

	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const;

};

#endif
