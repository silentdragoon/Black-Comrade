#ifndef _SWARMMANAGER_H
#define _SWARMMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <vector>
#include "ITickable.h"
#include "swarm.h"

using namespace Ogre;
using namespace std;

class SwarmManager : public ITickable
{
    private:

        SceneManager *sceneMgr;

        vector<Swarm*> *activeSwarms;

        int id;

    public:

        SwarmManager(SceneManager *sceneMgr);
        ~SwarmManager();

        void createSwarm(int size, Vector3 location);

        virtual void tick();

};

#endif
