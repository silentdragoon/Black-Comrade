#ifndef SYSTEMSSTATE_H
#define SYSTEMSSTATE_H

#include "ReplicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

class SystemsState : public ReplicaObject {
public:

	int rearShieldPower;
	int frontShieldPower;
	int enginePower;
	int weaponsPower;

	SystemsState ();

	void Print();

       void increaseFrontShieldPower();

       void increaseRearShieldPower();

       void increaseEnginePower();

       void increaseWeaponsPower();

	virtual RakNet::RakString GetName(void) const;

	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const;

	virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);

	virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

};

#endif
