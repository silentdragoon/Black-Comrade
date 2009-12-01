
#ifndef _BULLET_H
#define	_BULLET_H

#include <OGRE/Ogre.h>
#include <iostream>
#include "frontGunState.h"
#include "ITickable.h"

using namespace Ogre;

class Bullet : public ITickable
{
    private:

        SceneNode *shipSceneNode;
        SceneNode *bulletNode;
        SceneManager *sceneMgr;
        ParticleSystem *particle;
        FrontGunState *gunState;
        
        void fire();
    
    public:
        Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr, 
            FrontGunState *gunState);
            
        virtual void tick();
};


#endif

