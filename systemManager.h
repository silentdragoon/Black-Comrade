#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "ITickable.h"
#include "engineerControls.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"
#include <math.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include "damageState.h"

class SystemManager : public ITickable, public ReplicaObject {
private:
    double shieldRate; // From 0 - 1.0
    double weaponRate; // From 0 - 1.0
    double sensorRate; // From 0 - 1.0
    double engineRate; // From 0 - 3.0

    double weaponCharge; // 0 - 100 ?
    double shieldCharge; // 0 - 100 ?

    int timeSinceLastPress;

    EngineerControls *engCon;
    DamageState *damageState;

public:
    
    SystemManager();
    SystemManager(EngineerControls *engCon, DamageState *damageState);
    void tick();

    void incShieldRate();
    void incWeaponRate();
    void incSensorRate();

    double getShieldRate(); //
    double getWeaponRate(); //
    double getSensorRate(); //
    double getEngineRate(); // Returns 0 - 1

    void fireWeapon();
    void damageShield();
             
    double getWeaponCharge();
    double getShieldCharge();

    void transferShieldsToWeapons();
    void transferWeaponsToShields();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
