#ifndef _BULLETMANAGER_H
#define	_BULLETMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "frontGunState.h"
#include "ITickable.h"
#include "bullet.h"
#include "collisionManager.h"
#include "swarmManager.h"

using namespace Ogre;
using namespace std;

class BulletManager : public ITickable
{
    private:
        vector<Bullet*> *activeBullets;
        SceneNode *shipSceneNode;
        SceneManager *sceneMgr;
        int bnum;
        FrontGunState *gunState;
        CollisionManager *colMgr;
        SwarmManager *swarmMgr;
        
        void fire(SceneNode *bulletNode, Vector3 direction, string name, string rname, double distance);
        void updateBullets(); 
    public:
        BulletManager(SceneNode *shipSceneNode, SceneManager *sceneMgr, FrontGunState *gunState, CollisionManager *colMgr, SwarmManager *swarmMgr);

        ~BulletManager();
            
        virtual void tick();
};


#endif

