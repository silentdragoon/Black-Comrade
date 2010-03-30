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
        string name;
        string rname;
        SceneManager *sceneMgr;

        void makeNode();

        Vector3 origin;
        Vector3 direction;
        Vector3 deathSpark;
        int velocity;

               
   public:
        double distanceTravelled;
        double distanceToTravel;
        
        bool madeNoise;

        Bullet(IBulletOwner *owner, IBulletTarget *target,
               SceneManager *sceneMgr, double distanceToTravel);

        Bullet(SceneNode *bulletNode,
            SceneManager *sceneMgr,
            string name,
            string rname,
            Vector3 direction, 
            int velocity,
            double dtt,
            PlayerStats *playerStats); // Distance to travel

        ~Bullet();

        void updateLocation();
        Vector3 getOrigin();
        Vector3 getDeathSpark();

        IBulletOwner *getOwner();
        IBulletTarget *getTarget();

        SceneNode *getNode();
        
        Enemy *enemy;
        bool hitEnemy;
        bool hitShip;

        PlayerStats *playerStats;
};

#endif

