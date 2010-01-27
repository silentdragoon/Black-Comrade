#ifndef _SWARM_H 
#define _SWARM_H

#include <OGRE/Ogre.h>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "enemy.h"
#include "ITickable.h"
#include "rayQuery.h"

using namespace Ogre;
using namespace std;

class Swarm : public ITickable
{
    private:
        SceneManager *sceneMgr;
        SceneNode *leadSN;
        vector<Enemy*> members;
        int size;
        int id;
        Vector3 location;
    	Real roll;
    	Real pitch;
    	Real yaw;
    	RayQuery *rRayQuery;
    
    	void updateSwarmLocation();
    	void updateEnemyLocations();
    	
    public:

        Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
			Real roll, Real pitch, Real yaw);
        ~Swarm();

        Vector3 getAverageAlignment();
        Vector3 getAveragePosition();
        
        void tick();        
};

#endif
