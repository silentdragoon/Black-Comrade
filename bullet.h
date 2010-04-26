#ifndef _BULLET_H 
#define _BULLET_H

#include <OGRE/Ogre.h>
#include <string>
#include <iostream>
#include <math.h>
#include "enemy.h"
#include "playerStats.h"

#include "IBulletOwner.h"
#include "IBulletTarget.h"

using namespace Ogre;
using namespace std;

class Bullet
{
    private:

        IBulletOwner *owner;
        IBulletTarget *target;      

        SceneNode *node;
        RibbonTrail *trail;
        Light *l;

        SceneManager *sceneMgr;

        void makeNode();

        Vector3 origin;
        Vector3 direction;
        Vector3 pointOfImpact;

        int velocity;
        int damage;
               
   public:
        double distanceTravelled;
        double distanceToTravel;
        
        bool madeNoise;

        Bullet(IBulletOwner *owner, IBulletTarget *target,
               SceneManager *sceneMgr, double distanceToTravel);

        ~Bullet();

        void updateLocation();
        Vector3 getOrigin();
        Vector3 getPointOfImpact();

        int getDamage();

        IBulletOwner *getOwner();
        IBulletTarget *getTarget();
        
        Enemy *enemy;
        bool hitEnemy;
        bool hitShip;

        PlayerStats *playerStats;
};

#endif

