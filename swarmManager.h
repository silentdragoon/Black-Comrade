#ifndef _SWARMMANAGER_H
#define _SWARMMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include "ITickable.h"
#include "swarm.h"
#include "gameParameterMap.h"
#include "mapManager.h"
#include "shipState.h"
#include "collisionManager.h"

#include "networkingManager.h"

using namespace Ogre;
using namespace std;

class SwarmManager : public ITickable
{
    private:

        SceneManager *sceneMgr;
        SceneNodeManager *sceneNodeMgr;
        GameParameterMap *gamePM;
        MapManager *mapMgr;
        std::vector<Swarm*> activeSwarms;
        ShipState *shipState;
        CollisionManager *colMgr;
        NetworkingManager *networkingMgr;

        int id;

        int dynSwarmSize;

        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	MapManager *mapMgr, ShipState *shipState, CollisionManager* colMgr, NetworkingManager *networkingMgr);

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	CollisionManager* colMgr, NetworkingManager *networkingMgr);

        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        void updateRemoteSwarms();

        std::vector<Enemy*> getAllEnemies();
        std::vector<Enemy*> getReplicatedEnemies();

        virtual void tick();

};

#endif
