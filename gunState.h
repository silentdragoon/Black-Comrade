
#ifndef FRONT_GUN_STATE_H
#define FRONT_GUN_STATE_H

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "IPlayerControls.h"
#include "collaborationInfo.h"
#include "playerStats.h"
#include "damageState.h"
#include "systemManager.h"

// include Raknet classes
#include "replicaObject.h"
#include "ReplicaManager3.h"

using namespace RakNet;

class GunState : public ITickable, public ReplicaObject
{
    private:
        int timeSinceLastFire;
        bool isFire;

        GameRole owner;

        Vector3 position;
        Quaternion orientation;
        
        IPlayerControls *playerControls;
        DamageState *damageState;
        SystemManager *systemManager;
        
    public:
        bool fire();

        Vector3 getPosition();
        Quaternion getOrientation();
        void setSystemManager(SystemManager *sysMan);
        PlayerStats *stats;
        
        virtual void tick();
        
        GunState();
        GunState(IPlayerControls *pilotControls, DamageState *damageState, SystemManager *systemManger, CollaborationInfo *ownerInfo);
        ~GunState();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
        virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection);
        virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection);
};

#endif
