
#include "damageState.h"

#include "constManager.h"

#include <iostream>

DamageState::DamageState()
    : sensorHealth(100.0)
    , weaponHealth(100.0)
    , engineHealth(100.0)
    , hullHealth(100.0)
    , shieldModifier(0.1)
    , isDamaged(false)
    , pilotInfo(0)
    , engineerInfo(0)
    , navigatorInfo(0)
    , damageSustained(0)
{}

DamageState::DamageState(CollaborationInfo *pilotInfo,
                         CollaborationInfo *engineerInfo,
                         CollaborationInfo *navigatorInfo)
    : sensorHealth(100.0)
    , weaponHealth(100.0)
    , engineHealth(100.0)
    , hullHealth(100.0)
    , shieldModifier(0.1)
    , isDamaged(false)
    , pilotInfo(pilotInfo)
    , engineerInfo(engineerInfo)
    , navigatorInfo(navigatorInfo)
    , damageSustained(0)
{
    rng.seed(static_cast<unsigned int>(std::time(0)));
}

void DamageState::tick() {
    isDamaged = false;
    if (pilotInfo != 0) {
        checkForRepairs(pilotInfo);
    }
    if (engineerInfo != 0) {
        checkForRepairs(engineerInfo);
    }
    if (navigatorInfo != 0) {
        checkForRepairs(navigatorInfo);
    }
}

void DamageState::checkForRepairs(CollaborationInfo *repairer) {
    ShipSystem toRepair = repairer->toRepair;

    switch(toRepair) {
        case(SS_NONE):
            return;
        case (SS_ENGINES):
            repairEngines(repairer->repairAmount);
            break;
        case (SS_SENSORS):
            repairSensors(repairer->repairAmount);
            break;
        case (SS_WEAPONS):
            repairWeapons(repairer->repairAmount);
            break;
        case (SS_HULL):
            repairHull(repairer->repairAmount);
            break;
    }

    repairer->toRepair = SS_NONE;
    repairer->repairAmount = 0;
}

double DamageState::getSensorHealth() { return sensorHealth; }

double DamageState::getWeaponHealth() { return weaponHealth; }

double DamageState::getEngineHealth() { return engineHealth; }

double DamageState::getHullHealth() { return hullHealth; }

// Being shot
void DamageState::damage() {  
    damage(ConstManager::getFloat("enemy_bullet_damage"));
}

void DamageState::damage(double multiplier) {
    multiplier = fabs(multiplier);

    /*if (shieldHealth <= 0 && hullHealth > 0) {
        hullHealth = hullHealth - (multiplier * 4);
        if (hullHealth < 0) hullHealth = 0.0;
        //print();
        return;
    }*/

    boost::uniform_int<> six(1,4);
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(rng, six);
    int irand = die();
    
    int damage = (multiplier * 5 * shieldModifier);
    switch(irand) {
        case 1:
            if (sensorHealth > 0) {
                sensorHealth -= damage;
                if (sensorHealth < 0) sensorHealth = 0.0;
                break;
            }
        case 2:
            if (weaponHealth > 0) {
                weaponHealth -= damage;
                if (weaponHealth < 0) weaponHealth = 0.0;
                break;
            }
        case 3:
            if (engineHealth > 0) {
                engineHealth -= damage;
                if (engineHealth < 0) engineHealth = 0.0;
                break;
            }
        case 4:
            if (hullHealth > 0) {
                hullHealth -= damage;
                if (hullHealth < 0) hullHealth = 0.0;
                break;
            }
    }

    damageSustained += damage;
    isDamaged = true;
}

void DamageState::setShieldModifier(double mod) {
    shieldModifier = mod;
}

void DamageState::repairWeapons(int amount) {
    weaponHealth += amount;
    if (weaponHealth > 100) weaponHealth = 100;
}

void DamageState::repairSensors(int amount) {
    sensorHealth += amount;
    if (sensorHealth > 100) sensorHealth = 100;
}

void DamageState::repairEngines(int amount) {
    engineHealth += amount;
    if (engineHealth > 100) engineHealth = 100;
}

void DamageState::repairHull(int amount) {
    hullHealth += amount;
    if (hullHealth > 100) hullHealth = 100;
}

int DamageState::getDamageSustained() { return damageSustained; }

RakNet::RakString DamageState::GetName(void) const {return RakNet::RakString("DamageState");}

RM3SerializationResult DamageState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(shieldModifier);
    serializeParameters->outputBitstream[0].Write(sensorHealth);
    serializeParameters->outputBitstream[0].Write(weaponHealth);
    serializeParameters->outputBitstream[0].Write(engineHealth);
    serializeParameters->outputBitstream[0].Write(hullHealth);
    serializeParameters->outputBitstream[0].Write(isDamaged);
    serializeParameters->outputBitstream[0].Write(damageSustained);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void DamageState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shieldModifier);
    deserializeParameters->serializationBitstream[0].Read(sensorHealth);
    deserializeParameters->serializationBitstream[0].Read(weaponHealth);
    deserializeParameters->serializationBitstream[0].Read(engineHealth);
    deserializeParameters->serializationBitstream[0].Read(hullHealth);
    deserializeParameters->serializationBitstream[0].Read(damageSustained);

    bool isDamaged2 = false;
    deserializeParameters->serializationBitstream[0].Read(isDamaged2);
    if (isDamaged2) isDamaged = true;
}

void DamageState::print() {
    printf("DamageState is now (S:%.1f E:%.1f W:%.1f SE:%.1f H:%.1f)\n",
        shieldModifier,engineHealth,weaponHealth,sensorHealth,hullHealth);
}



