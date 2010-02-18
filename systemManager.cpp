#include "systemManager.h"

SystemManager::SystemManager(EngineerControls *engCon) :
    engCon(engCon),
    shieldRate(0.5),
    weaponRate(0.5),
    sensorRate(0.5),
    engineRate(1.5),
    weaponCharge(0),
    shieldCharge(0)
{}

void SystemManager::tick() {
    // TODO: Do the decharging and recharging here
    double decharge = 0.5;
    weaponCharge += (weaponRate-decharge);
    shieldCharge += (shieldRate-decharge);

    if(weaponCharge<0) weaponCharge = 0;
    if(shieldCharge<0) shieldCharge = 0;
    if(weaponCharge>100) weaponCharge = 100;
    if(shieldCharge>100) shieldCharge = 100;

    if(engCon->isShield()) {
        incShieldRate();
    }

    if(engCon->isWeapons()) {
        incWeaponRate();
    }

    if(engCon->isSensors()) {
        incSensorRate();
    }
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
    return engineRate;
}

void SystemManager::fireWeapon() {
    weaponCharge -= 0.1; // TODO: Fiddle this number or something
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
