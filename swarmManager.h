#ifndef _SWARMMANAGER_H
#define _SWARMMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include "ITickable.h"
#include "swarm.h"
#include "gameParameterMap.h"
#include "mapManager.h"

using namespace Ogre;
using namespace std;

class SwarmManager : public ITickable
{
    private:

        SceneManager *sceneMgr;
        GameParameterMap *gamePM; 
        MapManager *mapMgr;
        vector<Swarm*> *activeSwarms;

        int id;

    public:

        SwarmManager(SceneManager *sceneMgr, GameParameterMap *gamePM, MapManager *mapMgr);
        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        virtual void tick();

};

#endif
