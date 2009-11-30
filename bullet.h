
#ifndef _BULLET_H
#define	_BULLET_H

#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"

using namespace Ogre;


class Bullet : public ITickable
{
    private:

        SceneNode *shipSceneNode;
        SceneNode *bulletNode;
        SceneManager *sceneMgr;
        ParticleSystem *particle;
        

    public:

        void tick();
        Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr);
};


#endif

