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
#include "constManager.h"

class SystemManager : public ITickable, public ReplicaObject {
private:
    double shieldRate; // From 0 - 1.0
    double weaponRate; // From 0 - 1.0
    //double sensorRate; // From 0 - 1.0
    double engineRate; // From 0 - 2.0

    double weaponCharge; // 0 - 100 ?
    double shieldCharge; // 0 - 100 ?

    int timeSinceWepPress;
    int timeSinceEngPress;
    int timeSinceLastPress;

    int timeSinceWeaponRecharge;
    int timeSinceShieldRecharge;

    bool powersBeenChanged;

    EngineerControls *engCon;
    DamageState *damageState;

public:
    
    SystemManager();
    SystemManager(EngineerControls *engCon, DamageState *damageState);
    void tick();

    void incEngineRate();
    void incWeaponRate();
    //void incSensorRate();

    bool areWeaponsStuck();
    bool areShieldsStuck();

    double getShieldRate(); //
    double getWeaponRate(); //
    //double getSensorRate(); //
    double getEngineRate(); // Returns 0 - 1

    void fireWeapon();
    void damageShield();
             
    double getWeaponCharge();
    double getShieldCharge();

    void transferShieldsToWeapons();
    void transferWeaponsToShields();

    void resetPowersBeenChanged();
    bool havePowersBeenChanged();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
