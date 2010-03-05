#include "bullet.h"

void Bullet::updateLocation() {
    bulletNode->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    distanceTravelled = distanceTravelled + 
        sqrt(pow(direction.x*velocity,2)+pow(direction.y*velocity,2)+pow(direction.z*velocity,2)); 
}

Vector3 Bullet::getDeathSpark() {
    return deathSpark;
}

Bullet::Bullet(SceneNode *bulletNode,
        SceneManager *sceneMgr,
        string name,
        string rname,
        Vector3 direction,
        int velocity,
        double dtt)
        : 
            bulletNode(bulletNode),
            sceneMgr(sceneMgr),
            name(name), 
            rname(rname),
            direction(direction),
            velocity(velocity),
            distanceToTravel(dtt),
            distanceTravelled(0.0),
            enemy(0),
            hitEnemy(false),
            hitShip(false)
{
    deathSpark = Vector3(bulletNode->getPosition().x+(direction.x*dtt),bulletNode->getPosition().y+(direction.y*dtt),bulletNode->getPosition().z+(direction.z*dtt));
}

Bullet::~Bullet() {
    sceneMgr->destroySceneNode(name);
    sceneMgr->destroyRibbonTrail(rname);
}
        
