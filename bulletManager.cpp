
#include "bulletManager.h"

// This method needs to make a new bullet particle and add it to eg. a vector
void Bullet::fire()
{
    //if (particle != 0)
    //    sceneMgr->destroyParticleSystem(particle);
    string name = "Bullet";
    stringstream out;
    out << bnum++;
    name += out.str();
    ParticleSystem *particle = sceneMgr->createParticleSystem(name, "PE/Bullet");
    bulletNode->attachObject(particle);
}

Bullet::Bullet(SceneNode *shipSceneNode,SceneManager *sceneMgr, FrontGunState *gunState) 
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , gunState(gunState)
    , bnum(0)
{
    bulletNode = shipSceneNode->createChildSceneNode();
}

void Bullet::tick()
{
    if(gunState->fire()) fire();
}
