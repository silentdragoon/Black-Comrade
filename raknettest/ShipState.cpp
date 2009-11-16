#include "ShipState.h"
#include "ReplicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

	ShipState::ShipState()
       {
		position = new Vector3(0.0,0.0,-500.0);
		roll=0.0;
		pitch=0.0;
		yaw=0.0;
	}

	RakNet::RakString ShipState::GetName(void) const {return RakNet::RakString("ShipState");}

	void ShipState::WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const {
		allocationIdBitstream->Write(GetName());
	}

	RM3SerializationResult ShipState::Serialize(SerializeParameters *serializeParameters)	{
		serializeParameters->outputBitstream[0].Write(position->x);
		serializeParameters->outputBitstream[0].Write(position->y);
		serializeParameters->outputBitstream[0].Write(position->z);
		serializeParameters->outputBitstream[0].Write(roll);
		serializeParameters->outputBitstream[0].Write(pitch);
		serializeParameters->outputBitstream[0].Write(yaw);

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	void ShipState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		deserializeParameters->serializationBitstream[0].Read(position->x);	
		deserializeParameters->serializationBitstream[0].Read(position->y);	
		deserializeParameters->serializationBitstream[0].Read(position->z);	
		deserializeParameters->serializationBitstream[0].Read(roll);
		deserializeParameters->serializationBitstream[0].Read(pitch);
		deserializeParameters->serializationBitstream[0].Read(yaw);

		Print();
	}

	void ShipState::Print() {
		printf("ShipState is now (X:%.1f Y:%.1f Z:%.1f), (R:%.1f P:%.1f Y:%.1f)\n", position->x, position->y, position->z, roll, pitch, yaw);
	}
