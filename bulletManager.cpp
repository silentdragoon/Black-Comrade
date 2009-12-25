#include "bulletManager.h"

void BulletManager::fire(SceneNode *bulletNode) {
    
    Bullet *b = new Bullet(bulletNode,Vector3(1,0,0),2,false,false);
    
    activeBullets.push_back(*b);
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets.size();i++) {
        Bullet b = activeBullets.at(i);
        b.updateLocation();
        // TODO: Test for a collision/Destruction here
    }
}

BulletManager::BulletManager(SceneNode *shipSceneNode,SceneManager *sceneMgr, FrontGunState *gunState) 
    : shipSceneNode(shipSceneNode)
    , sceneMgr(sceneMgr)
    , gunState(gunState)
    , bnum(0)
{}

void BulletManager::tick()
{
    if(gunState->fire()) {

        // Making a new bullet
        string name = "Bullet";
        stringstream out;
        out << bnum++;
        name += out.str();

        SceneNode *bulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
        bulletNode->setPosition(shipSceneNode->getPosition());

        // TODO: Get the direction the ship is pointing in first then pass it to the fire method
        
        ParticleSystem *particle = sceneMgr->createParticleSystem(name, "PE/Bullet");
        bulletNode->attachObject(particle);
        
        fire(bulletNode);
    }
    updateBullets();
}
