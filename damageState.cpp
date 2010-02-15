
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

double DamageState::getShieldHealth() { return shieldHealth; }

double DamageState::getSensorHealth() { return sensorHealth; }

double DamageState::getWeaponHealth() { return weaponHealth; }

double DamageState::getEngineHealth() { return engineHealth; }

double DamageState::getHullHealth() { return hullHealth; }

void DamageState::damage() {  
    damage(1.0);
}

void DamageState::damage(double multiplier) {
    multiplier = fabs(multiplier);

    if (shieldHealth <= 0 && hullHealth > 0) {
        hullHealth = hullHealth - (multiplier * 4);
        if (hullHealth < 0) hullHealth = 0.0;
        //print();
        return;
    }

    srand ( time(NULL) );
    int irand = rand() % 4 + 1;
    
    switch(irand) {
        case 1:
            if (shieldHealth > 0) {
                shieldHealth = shieldHealth - (multiplier * 4);
                if (shieldHealth < 0) shieldHealth = 0.0;
                break;
            }
        case 2:
            if (sensorHealth > 0) {
                sensorHealth = sensorHealth - (multiplier * 5);
                if (sensorHealth < 0) sensorHealth = 0.0;
                break;
            }
        case 3:
            if (weaponHealth > 0) {
                weaponHealth = weaponHealth - (multiplier * 5);
                if (weaponHealth < 0) weaponHealth = 0.0;
                break;
            }
        case 4:
            if (engineHealth > 0) {
                engineHealth = engineHealth - (multiplier * 5);
                if (engineHealth < 0) engineHealth = 0.0;
                break;
            }
    }

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



