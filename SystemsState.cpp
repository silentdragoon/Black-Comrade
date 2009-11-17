#include "SystemsState.h"
#include "ReplicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

	SystemsState::SystemsState()
       {
		rearShieldPower = 25;
		frontShieldPower = 25;
		enginePower = 25;
		weaponsPower = 25;
	}

	RakNet::RakString SystemsState::GetName(void) const {return RakNet::RakString("SystemsState");}

	void SystemsState::WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const {
		allocationIdBitstream->Write(GetName());
	}

	RM3SerializationResult SystemsState::Serialize(SerializeParameters *serializeParameters)	{
		serializeParameters->outputBitstream[0].Write(rearShieldPower);
		serializeParameters->outputBitstream[0].Write(frontShieldPower);
		serializeParameters->outputBitstream[0].Write(enginePower);
		serializeParameters->outputBitstream[0].Write(weaponsPower);

		return RM3SR_BROADCAST_IDENTICALLY;
	}

	void SystemsState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
		deserializeParameters->serializationBitstream[0].Read(rearShieldPower);	
		deserializeParameters->serializationBitstream[0].Read(frontShieldPower);	
		deserializeParameters->serializationBitstream[0].Read(enginePower);	
		deserializeParameters->serializationBitstream[0].Read(weaponsPower);

		Print();
	}

	void SystemsState::Print() {
		printf("SystemsState is now (RS:%i FS:%i E:%i W:%i)\n", rearShieldPower, frontShieldPower, enginePower, weaponsPower);
	}

       void SystemsState::increaseFrontShieldPower() {
       	if (rearShieldPower != 0)
		{
			frontShieldPower = frontShieldPower + 1;
			rearShieldPower = rearShieldPower - 1;
		}
	}

       void SystemsState::increaseRearShieldPower() {
       	if (frontShieldPower != 0)
		{
			frontShieldPower = frontShieldPower - 1;
			rearShieldPower = rearShieldPower + 1;
		}
	}

       void SystemsState::increaseEnginePower() {
       	if (weaponsPower != 0)
		{
			weaponsPower = weaponsPower - 1;
			enginePower = enginePower + 1;
		}
	}

       void SystemsState::increaseWeaponsPower() {
       	if (enginePower != 0)
		{
			weaponsPower = weaponsPower + 1;
			enginePower = enginePower - 1;
		}
	}
