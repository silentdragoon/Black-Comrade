#ifndef DAMAGE_STATE_H
#define DAMAGE_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"
#include <math.h>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace Ogre;
using namespace RakNet;

class DamageState : public ITickable, public ReplicaObject {
private:
    double shieldHealth;
    double sensorHealth;
    double weaponHealth;
    double engineHealth;
    double hullHealth;

public:
   
    DamageState();

    void print();
    void tick();
    void damage();

    double getShieldHealth();
    double getSensorHealth();
    double getWeaponHealth();
    double getEngineHealth();
    double getHullHealth();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
