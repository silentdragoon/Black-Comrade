
#ifndef _BULLET_H
#define	_BULLET_H

#include <OGRE/Ogre.h>
#include <iostream>

using namespace Ogre;


class Bullet
{
    private:

        SceneNode *shipSceneNode;
        SceneNode *bulletNode;
        SceneManager *sceneMgr;
        ParticleSystem *particle;
        

    public:
        Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr);
        void fire();

};


#endif

