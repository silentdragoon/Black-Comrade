#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <OGRE/Ogre.h>
#include "ITickable.h"
#include "engineerControls.h"

using namespace Ogre;

class SystemManager : public ITickable {
private:
    double shieldRate; // From 0 - 1.0
    double weaponRate; // From 0 - 1.0
    double sensorRate; // From 0 - 1.0
    double engineRate; // From 0 - 3.0

    double weaponCharge; // 0 - 100 ?
    double shieldCharge; // 0 - 100 ?

    int timeSinceLastPress;

    EngineerControls *engCon;
public:
    
    SystemManager(EngineerControls *engCon);
    void tick();

    void incShieldRate();
    void incWeaponRate();
    void incSensorRate();

    double getShieldRate();
    double getWeaponRate();
    double getSensorRate();
    double getEngineRate();

    void fireWeapon();
    void damageShield();
             
    double getWeaponCharge();
    double getShieldCharge();

    void transferShieldsToWeapons();
    void transferWeaponsToShields();
};

#endif
