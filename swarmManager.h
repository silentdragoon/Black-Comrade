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
#include "lines.h"

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
		Lines *lines;
        int id;

        int dynSwarmSize;

        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
        	MapManager *mapMgr, ShipState *shipState, CollisionManager* colMgr);
        SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, CollisionManager *colMgr);
        ~SwarmManager();

        void createSwarm(int size, Vector3 location);
        void createSwarm(std::vector<Enemy*> enemies);

        std::vector<Enemy*> getAllEnemies();

        virtual void tick();

};

#endif
