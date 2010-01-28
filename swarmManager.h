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

using namespace Ogre;
using namespace std;

class SwarmManager : public ITickable
{
    private:

        SceneManager *sceneMgr;
        GameParameterMap *gamePM;
        MapManager *mapMgr;
        vector<Swarm*> activeSwarms;
        ShipState *shipState;

        int id;

        int dynSwarmSize;

        int swarmTick;

    public:

        SwarmManager(SceneManager *sceneMgr, GameParameterMap *gamePM, 
        	MapManager *mapMgr, ShipState *shipState);
        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        vector<Enemy*> getAllEnemies();

        virtual void tick();

};

#endif
