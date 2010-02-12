
#include "damageState.h"

#include <iostream>

DamageState::DamageState()
    : shieldHealth(100.0)
    , sensorHealth(100.0)
    , weaponHealth(100.0)
    , engineHealth(100.0)
{}

void DamageState::tick() {
    //print();
}

RakNet::RakString DamageState::GetName(void) const {return RakNet::RakString("DamageState");}

RM3SerializationResult DamageState::Serialize(SerializeParameters *serializeParameters) {
    //serializeParameters->outputBitstream[0].Write(position->x);


    return RM3SR_BROADCAST_IDENTICALLY;
}

void DamageState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    //deserializeParameters->serializationBitstream[0].Read(position->x);	


    //print();
}

void DamageState::print() {
    printf("DamageState is now (S:%.1f E:%.1f W:%.1f SE:%.1f)\n", shieldHealth,engineHealth,weaponHealth,sensorHealth);
}



