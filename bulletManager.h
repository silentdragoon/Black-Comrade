
#ifndef _BULLET_H
#define	_BULLET_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "frontGunState.h"
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class BulletManager : public ITickable
{
    private:
        vector<SceneNode> activeBullets;
        SceneNode *shipSceneNode;
        SceneManager *sceneMgr;
        //ParticleSystem *particle;
        int bnum;
        FrontGunState *gunState;
        
        void fire();
        void updateBullets(); 
    public:
        BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, 
            FrontGunState *gunState);
            
        virtual void tick();
};


#endif

