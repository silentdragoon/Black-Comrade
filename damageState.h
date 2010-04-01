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

#include <boost/math/distributions/uniform.hpp>
#include <boost/random.hpp>

using namespace Ogre;
using namespace RakNet;

class DamageState : public ITickable, public ReplicaObject {
private:

    boost::mt19937 rng;

    double shieldModifier;

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

    void setShieldModifier(double mod);

    void repairEngines(int amount);
    void repairWeapons(int amount);
    void repairSensors(int amount);
    void repairHull(int amount);

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
