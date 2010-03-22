#ifndef _SWARM_H 
#define _SWARM_H

#include <OGRE/Ogre.h>
#include <string>
#include <deque>
#include <sstream>
#include <iostream>
#include <math.h>
#include "enemy.h"
#include "rayQuery.h"
#include "shipState.h"
#include "sceneNodeManager.h"
#include "lines.h"
#include "collisionManager.h"
#include "constManager.h"
#include "mapManager2.h"
#include "pathFinder.h"
#include "gameParameterMap.h"
#include "particleSystemEffectManager.h"

using namespace Ogre;
using namespace std;

enum SwarmState { SS_PATROL, SS_ATTACK };

class Swarm
{
    private:
        SceneManager *sceneMgr;
        SceneNodeManager *sceneNodeMgr;
        std::deque<Enemy*> members;
        std::vector<MapTile*> path;
        int id;
        Vector3 location;
        Vector3 target;
        MapTile* currentSwarmTile;
        MapTile* oldShipTile;
        MapTile* oldSwarmTile;
    	Real roll;
    	Real pitch;
    	Real yaw;
    	RayQuery *rRayQuery;
    	float speed;
    	SwarmState state;
    	ShipState *shipState;
    	Lines *lines;
    	CollisionManager *collisionMgr;
        MapManager *mapMgr;
        PathFinder *pathFinder;
        GameParameterMap *gameParameterMap;
        ParticleSystemEffectManager *particleSystemEffectManager;
    
        void updateTargetLocation();
        void updateEnemyLocationsFlocking();
        void updateEnemyLocationsAttack();

    	void removeDeadEnemies();
    	void markDeadEnemies();
    	
    	void shootAtShip();
    	void pointAtShip(Enemy *e);
    	
    	void turnEnemy(Enemy *e);
    	void attackProcess(Enemy *e);
    	
    	float calcNewAngle(float old, float target, float step);
    	
    public:
        int size;

        Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
			Real roll, Real pitch, Real yaw, ShipState *shipState, SceneNodeManager *sceneNodeMgr, Lines *lines,
            CollisionManager *collisionMgr, MapManager *mapMgr, GameParameterMap *gameParameterMap,
            ParticleSystemEffectManager *particleSystemEffectManager);
        ~Swarm();

        Vector3 getAverageAlignment();
        Vector3 getAveragePosition();
    	bool isShipInSight();

        std::vector<Enemy*> getAllEnemies();
        
        void tick();

};

#endif
