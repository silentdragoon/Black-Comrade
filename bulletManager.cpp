#include "bulletManager.h"

void BulletManager::fire() {
    // Making a new bullet
    string name = "Bullet";
    stringstream out;
    out << bnum++;
    name += out.str();
    SceneNode *bulletNode = shipSceneNode->createChildSceneNode();
    ParticleSystem *particle = sceneMgr->createParticleSystem(name, "PE/Bullet");
    bulletNode->attachObject(particle);
    
    Bullet *b = new Bullet(bulletNode,Vector3(0,0,1),500,false,false);
    
    activeBullets.push_back(*b);
}

void BulletManager::updateBullets() {
    // Update the position of all the bullets in the vector. Somehow test to see if they
    // have collided with anything, or if they have run their course. If this is the case
    // we need to destroy them and remove them from the vector.
}

BulletManager::BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, FrontGunState *gunState) 
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , gunState(gunState)
    , bnum(0)
{
}

void BulletManager::tick()
{
    if(gunState->fire()) fire();
    updateBullets();
}
