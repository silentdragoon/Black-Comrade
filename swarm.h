#ifndef _SWARM_H 
#define _SWARM_H

#include <OGRE/Ogre.h>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "enemy.h"
#include "rayQuery.h"
#include "shipState.h"
#include "sceneNodeManager.h"

using namespace Ogre;
using namespace std;

enum SwarmState { SS_PATROL, SS_ATTACK };

class Swarm
{
    private:
        SceneManager *sceneMgr;
        SceneNodeManager *sceneNodeMgr;
        std::vector<Enemy*> members;
        int id;
        Vector3 location;
    	Real roll;
    	Real pitch;
    	Real yaw;
    	RayQuery *rRayQuery;
    	float speed;
    	SwarmState state;
    	ShipState *shipState;
    
    	void updateSwarmLocation();
    	void updateEnemyLocations();

    	void removeDeadEnemies();
    	void shootAtShip();
    	
    	bool isShipInSight();
    	
    public:
        int size;

        Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
			Real roll, Real pitch, Real yaw, ShipState *shipState, SceneNodeManager *sceneNodeMgr);
        ~Swarm();

        Vector3 getAverageAlignment();
        Vector3 getAveragePosition();

        std::vector<Enemy*> getAllEnemies();
        
        void tick();

};

#endif
