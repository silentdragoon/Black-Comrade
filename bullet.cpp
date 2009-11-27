#include "bullet.h"

void Bullet::tick()
{

}

Bullet::Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr) :
    shipSceneNode(shipSceneNode),
    sceneMgr(sceneMgr)
{
    sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(
    sceneMgr->createParticleSystem("Bullet", "particles/Bullet"));
}
