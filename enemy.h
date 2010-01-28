#ifndef _ENEMY_H 
#define _ENEMY_H

#include <OGRE/Ogre.h>
#include <string>
#include <iostream>
#include <math.h>

using namespace Ogre;
using namespace std;

class Enemy
{
    private:
        SceneManager *sceneMgr;
        
    public:
    	int health;
    	bool fire;
    	int fireDelay;
    
        SceneNode *node;
        Enemy(SceneNode *node, int health, SceneManager *sceneMgr);

        Vector3 getDirection();
        Vector3 getLocation();
        
        void setLocation(Vector3 v);
        void setOrientation(Real roll, Real pitch, Real yaw);
        
        Entity *getEntity();
        
        int getHealth();

        ~Enemy();
};

#endif
