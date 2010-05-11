#include "systemManager.h"

SystemManager::SystemManager() :
    engCon(0),
    shieldRate(1.0),
    weaponRate(0.5),
    engineRate(0.5),
    weaponCharge(100),
    shieldCharge(100),
    timeSinceWepPress(100),
    timeSinceEngPress(100),
    timeSinceLastPress(100),
    timeSinceWeaponRecharge(0),
    timeSinceShieldRecharge(0)
{
}

SystemManager::SystemManager(EngineerControls *engCon, DamageState *damageState) :
    engCon(engCon),
    damageState(damageState),
    shieldRate(1.0),
    weaponRate(0.5),
    engineRate(0.5),
    weaponCharge(100),
    shieldCharge(100),
    timeSinceWepPress(100),
    timeSinceEngPress(100),
    timeSinceLastPress(100),
    timeSinceWeaponRecharge(0),
    timeSinceShieldRecharge(0)
{
}

void SystemManager::tick() {
    if(engCon!=0) {
        timeSinceLastPress++;
        timeSinceWepPress++;
        timeSinceEngPress++;
        timeSinceWeaponRecharge++;
        timeSinceShieldRecharge++;

        double chargeModifier = 0.1;
        double decharge = 0.0;

        double weaponDiff = (weaponRate-decharge)*chargeModifier;
        double shieldDiff = (shieldRate/2.0-decharge)*chargeModifier;
        weaponCharge += weaponDiff;
        shieldCharge += shieldDiff;

        if (weaponDiff > 0.0) timeSinceWeaponRecharge = 0;
        if (shieldDiff > 0.0) timeSinceShieldRecharge = 0;

        if(weaponCharge<0) weaponCharge = 0;
        if(shieldCharge<0) shieldCharge = 0;
        if(weaponCharge>100) weaponCharge = 100;
        if(shieldCharge>100) shieldCharge = 100;

        if (damageState->isDamaged) damageShield();

        if((engCon->isEngine())&&(timeSinceEngPress>10)) {
            incEngineRate();
            timeSinceEngPress=0;
        }

        if((engCon->isWeapons())&&(timeSinceWepPress>10)) {
            incWeaponRate();

            timeSinceWepPress=0;
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
}

bool SystemManager::areWeaponsStuck() {
    return (timeSinceWeaponRecharge >= 350 && getWeaponCharge() == 0.0);
}

bool SystemManager::areShieldsStuck() {
    return (timeSinceShieldRecharge >= 350 && getShieldCharge() == 0.0);
}

void SystemManager::incEngineRate() {
    engineRate += 0.25;
    if(engineRate>1.0) {
        engineRate = 0;
        shieldRate += 1.0;
    } else {
        shieldRate -= 0.25;
    }
}

void SystemManager::incWeaponRate() {
    weaponRate += 0.25;
    if(weaponRate>1.0) {
        weaponRate = 0;
        shieldRate += 1.0;
    } else {
        shieldRate -= 0.25;
    }
}

double SystemManager::getShieldRate() {
    return shieldRate/2.0;
}

double SystemManager::getWeaponRate() {
    return weaponRate;
}

double SystemManager::getEngineRate() {
    return engineRate;
}

void SystemManager::fireWeapon() {
    weaponCharge -= 0.5; // TODO: Fiddle this number or something
}

void SystemManager::damageShield() {
    //std::cout << shieldCharge << std::endl;
    shieldCharge -= ConstManager::getInt("enemy_bullet_damage"); // TODO: fix this
    double mod = 1.0 - (tan(((double)shieldCharge / 100.0))/1.56);
    damageState->setShieldModifier(mod) ;
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
    serializeParameters->outputBitstream[0].Write(engineRate);
    serializeParameters->outputBitstream[0].Write(shieldCharge);
    serializeParameters->outputBitstream[0].Write(weaponCharge);
    return RM3SR_BROADCAST_IDENTICALLY;
}

void SystemManager::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(shieldRate);
    deserializeParameters->serializationBitstream[0].Read(weaponRate);
    deserializeParameters->serializationBitstream[0].Read(engineRate);
    deserializeParameters->serializationBitstream[0].Read(shieldCharge);
    deserializeParameters->serializationBitstream[0].Read(weaponCharge);
}
