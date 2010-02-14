
#include "damageState.h"

#include <iostream>

DamageState::DamageState()
    : shieldHealth(100.0)
    , sensorHealth(100.0)
    , weaponHealth(100.0)
    , engineHealth(100.0)
    , hullHealth(100.0)
{}

void DamageState::tick() {
    //print();
}

void DamageState::damage() {
    //shieldHealth = shieldHealth - 5;
    //print();
}

RakNet::RakString DamageState::GetName(void) const {return RakNet::RakString("DamageState");}

RM3SerializationResult DamageState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(shieldHealth);
    serializeParameters->outputBitstream[0].Write(sensorHealth);
    serializeParameters->outputBitstream[0].Write(weaponHealth);
    serializeParameters->outputBitstream[0].Write(engineHealth);
    serializeParameters->outputBitstream[0].Write(hullHealth);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void DamageState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shieldHealth);
    deserializeParameters->serializationBitstream[0].Read(sensorHealth);
    deserializeParameters->serializationBitstream[0].Read(weaponHealth);
    deserializeParameters->serializationBitstream[0].Read(engineHealth);
    deserializeParameters->serializationBitstream[0].Read(hullHealth);

    //print();
}

void DamageState::print() {
    printf("DamageState is now (S:%.1f E:%.1f W:%.1f SE:%.1f H:%.1f)\n",
        shieldHealth,engineHealth,weaponHealth,sensorHealth,hullHealth);
}



