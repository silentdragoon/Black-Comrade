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

using namespace Ogre;
using namespace std;

class SwarmManager : public ITickable
{
    private:

        SceneManager *sceneMgr;
        GameParameterMap *gamePM;
        MapManager *mapMgr;
        std::vector<Swarm*> activeSwarms;
        ShipState *shipState;
        CollisionManager *colMgr;

        int id;

        int dynSwarmSize;

        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, GameParameterMap *gamePM, 
        	MapManager *mapMgr, ShipState *shipState, CollisionManager* colMgr);
        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        std::vector<Enemy*> getAllEnemies();

        virtual void tick();

};

#endif
