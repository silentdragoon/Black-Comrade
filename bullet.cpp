#include "bullet.h"

void Bullet::fire()
{
    if (particle != 0)
        sceneMgr->destroyParticleSystem(particle);
    particle = sceneMgr->createParticleSystem("Bullet", "PE/Bullet");
    bulletNode->attachObject(particle);
    
}

Bullet::Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr) :
    shipSceneNode(shipSceneNode),
    sceneMgr(sceneMgr),
    particle(0)
{
    bulletNode = shipSceneNode->createChildSceneNode();
}
