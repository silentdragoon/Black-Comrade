#ifndef SHIPSTATE_H
#define SHIPSTATE_H

#include "ReplicaObject.h"
#include "ReplicaManager3.h"

#include <OGRE/Ogre.h>

using namespace RakNet;
using namespace Ogre;

class ShipState : public ReplicaObject {
public:
	Vector3 *position;
	double roll;
	double pitch;
	double yaw;

	ShipState ();

	void Print();

	virtual RakNet::RakString GetName(void) const;

	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const;

	virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

};

#endif
