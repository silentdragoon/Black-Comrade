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
        SceneNode *node;
        int health;
    public:

        Enemy(SceneNode *node, int health);

        Vector3 getDirection();
        Vector3 getLocation();
        
        void setLocation(Vector3 v);
        void setOrientation(Real roll, Real pitch, Real yaw);
        
        int getHealth();

        ~Enemy();
};

#endif

