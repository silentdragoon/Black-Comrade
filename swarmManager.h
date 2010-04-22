#ifndef _SWARMMANAGER_H
#define _SWARMMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include <deque>
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
        std::deque<Swarm*> activeSwarms;
        ShipState *shipState;
        CollisionManager *colMgr;
        NetworkingManager *networkingMgr;
        Lines *lines;
        GameStateMachine *gameStateMachine;
        ParticleSystemEffectManager *particleSystemEffectManager;
        SoundManager *soundMgr;

        std::vector<ReplicaObject*> replicatedEnemies;

        int id;

        int dynSwarmSize;

        int ticksSinceLastUpdate;
        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	MapManager *mapMgr, ShipState *shipState, CollisionManager* colMgr, NetworkingManager *networkingMgr,
            Lines *lines, GameStateMachine *gameStateMachine,
            ParticleSystemEffectManager *particleSystemEffectManager, SoundManager *soundMgr);

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	NetworkingManager *networkingMgr,ParticleSystemEffectManager *particleSystemEffectManager,
            SoundManager *soundMgr, CollisionManager *colMgr);

        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        void updateRemoteSwarms();

        std::vector<Enemy*> getAllLocalEnemies();
        std::vector<Enemy*> getReplicatedEnemies();
        
        std::deque<Swarm*> getAllSwarms();

        virtual void tick();

};

#endif
