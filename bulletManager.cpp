
#include "bulletManager.h"

void BulletManager::fire() {
    // Need to put all this stuff into a new class which is an actual bullet,
    // it needs to store the direction it was fired in and the type of bullet it is.
    // We can then move it according to the constants given using the updateBullets method.
    string name = "Bullet";
    stringstream out;
    out << bnum++;
    name += out.str();
    bulletNode = shipSceneNode->createChildSceneNode();
    ParticleSystem *particle = sceneMgr->createParticleSystem(name, "PE/Bullet");
    bulletNode->attachObject(particle);
    activeBullets.push_back(bulletNode);
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
