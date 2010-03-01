#ifndef DAMAGE_STATE_H
#define DAMAGE_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"

// include Raknet classes
#include "replicaObject.h"
#include "collaborationInfo.h"
#include "ReplicaManager3.h"
#include "shipSystem.h"
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

    CollaborationInfo *pilotInfo;
    CollaborationInfo *engineerInfo;
    CollaborationInfo *navigatorInfo;

    void checkForRepairs(CollaborationInfo *repairer);

public:

    DamageState(CollaborationInfo *pilotInfo,CollaborationInfo *engineerInfo,CollaborationInfo *navigatorInfo);
    DamageState();

    void print();
    void tick();
    void damage();
    void damage(double multiplier);

    void repairShieldGenerator(int amount);

    double getShieldHealth();
    double getSensorHealth();
    double getWeaponHealth();
    double getEngineHealth();
    double getHullHealth();

    bool isDamaged;

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
