#ifndef _SWARMMANAGER_H
#define _SWARMMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include "ITickable.h"
#include "swarm.h"
#include "gameParameterMap.h"
#include "mapManager2.h"
#include "shipState.h"
#include "collisionManager.h"
#include "lines.h"
#include "gameStateMachine.h"
#include "particleSystemEffectManager.h"

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
        Lines *lines;
        GameStateMachine *gameStateMachine;
        ParticleSystemEffectManager *particleSystemEffectManager;

        int id;

        int dynSwarmSize;

        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	MapManager *mapMgr, ShipState *shipState, CollisionManager* colMgr, NetworkingManager *networkingMgr,
            Lines *lines, GameStateMachine *gameStateMachine,
            ParticleSystemEffectManager *particleSystemEffectManager);

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	NetworkingManager *networkingMgr);

        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        void updateRemoteSwarms();

        std::vector<Enemy*> getAllEnemies();
        std::vector<Enemy*> getReplicatedEnemies();
        
        std::vector<Swarm*> getAllSwarms();

        virtual void tick();

};

#endif
