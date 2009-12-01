#include "bullet.h"

void Bullet::fire()
{
    particle = sceneMgr->createParticleSystem("Bullet", "PE/Bullet");
    bulletNode->attachObject(particle);
}

Bullet::Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr) :
    shipSceneNode(shipSceneNode),
    sceneMgr(sceneMgr)
{
    bulletNode = shipSceneNode->createChildSceneNode();
}
