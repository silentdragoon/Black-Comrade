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
        
        void fire(SceneNode *bulletNode, Vector3 direction, string name, string rname);
        void updateBullets(); 
    public:
        BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, 
            FrontGunState *gunState);

        ~BulletManager();
            
        virtual void tick();
};


#endif

