#include "systemManager.h"

SystemManager::SystemManager() :
    engCon(0),
    shieldRate(0.5),
    weaponRate(0.5),
    sensorRate(0.5),
    engineRate(1.5),
    weaponCharge(100),
    shieldCharge(100),
    timeSinceLastPress(100)
{
}

SystemManager::SystemManager(EngineerControls *engCon, DamageState *damageState) :
    engCon(engCon),
    damageState(damageState),
    shieldRate(0.5),
    weaponRate(0.5),
    sensorRate(0.5),
    engineRate(1.5),
    weaponCharge(100),
    shieldCharge(100),
    timeSinceLastPress(100)
{
}

void SystemManager::tick() {
    if(engCon!=0) {
        timeSinceLastPress++;

        double chargeModifier = 0.1;

        double decharge = 0.5;
        weaponCharge += (weaponRate-decharge)*chargeModifier;
        shieldCharge += (shieldRate-decharge)*chargeModifier;

        if(weaponCharge<0) weaponCharge = 0;
        if(shieldCharge<0) shieldCharge = 0;
        if(weaponCharge>100) weaponCharge = 100;
        if(shieldCharge>100) shieldCharge = 100;

        if (damageState->isDamaged) damageShield();

        if((engCon->isShield())&&(timeSinceLastPress>10)) {
            incShieldRate();
            timeSinceLastPress=0;
        }

        if((engCon->isWeapons())&&(timeSinceLastPress>10)) {
            incWeaponRate();
            timeSinceLastPress=0;
        }

        if((engCon->isSensors())&&(timeSinceLastPress>10)) {
            incSensorRate();
            timeSinceLastPress=0;
        }

        if((engCon->transferShields())&&(timeSinceLastPress>10)) {
            transferWeaponsToShields();
            timeSinceLastPress=0;
        }

        if((engCon->transferWeapons())&&(timeSinceLastPress>10)) {
            transferShieldsToWeapons();
            timeSinceLastPress=0;
        }
    }

    std::cout << shieldCharge << std::endl;
}

void SystemManager::incShieldRate() {
    shieldRate += 0.25;
    if(shieldRate>1.0) {
        shieldRate = 0;
        engineRate += 1.0;
    } else {
        engineRate -= 0.25;
    }
}

void SystemManager::incWeaponRate() {
    weaponRate += 0.25;
    if(weaponRate>1.0) {
        weaponRate = 0;
        engineRate += 1.0;
    } else {
        engineRate -= 0.25;
    }
}

void SystemManager::incSensorRate() {
    sensorRate += 0.25;
    if(sensorRate>1.0) {
        sensorRate = 0;
        engineRate += 1.0;
    } else {
        engineRate -= 0.25;
    }
}

double SystemManager::getShieldRate() {
    return shieldRate;
}

double SystemManager::getWeaponRate() {
    return weaponRate;
}

double SystemManager::getSensorRate() {
    return sensorRate;
}

double SystemManager::getEngineRate() {
    double blarg = engineRate/3.0;
    return blarg;
}

void SystemManager::fireWeapon() {
    weaponCharge -= 1.0; // TODO: Fiddle this number or something
}

void SystemManager::damageShield() {
    shieldCharge -= 10; // TODO: fix this
}

double SystemManager::getWeaponCharge() {
    return weaponCharge;
}

double SystemManager::getShieldCharge() {
    return shieldCharge;
}

void SystemManager::transferShieldsToWeapons() {
    if(shieldCharge>19) {
        shieldCharge -= 20;
        weaponCharge += 20;
        if(weaponCharge>100) weaponCharge = 100;
    } else {
        weaponCharge = weaponCharge + shieldCharge;
        shieldCharge = 0;
        if(weaponCharge>100) weaponCharge = 100;
    }
}

void SystemManager::transferWeaponsToShields() {
    if(weaponCharge>19) {
        weaponCharge -= 20;
        shieldCharge += 20;
        if(shieldCharge>100) shieldCharge = 100;
    } else {
        shieldCharge = shieldCharge + weaponCharge;
        weaponCharge = 0;
        if(shieldCharge>100) shieldCharge = 100;
    }
}

RakNet::RakString SystemManager::GetName(void) const{return RakNet::RakString("SystemManager");}

RM3SerializationResult SystemManager::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(shieldRate);
    serializeParameters->outputBitstream[0].Write(weaponRate);
    serializeParameters->outputBitstream[0].Write(sensorRate);
    serializeParameters->outputBitstream[0].Write(engineRate);
    serializeParameters->outputBitstream[0].Write(shieldCharge);
    serializeParameters->outputBitstream[0].Write(weaponCharge);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void SystemManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shieldRate);
    deserializeParameters->serializationBitstream[0].Read(weaponRate);
    deserializeParameters->serializationBitstream[0].Read(sensorRate);
    deserializeParameters->serializationBitstream[0].Read(engineRate);
    deserializeParameters->serializationBitstream[0].Read(shieldCharge);
    deserializeParameters->serializationBitstream[0].Read(weaponCharge);
}
