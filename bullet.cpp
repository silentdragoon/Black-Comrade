#include "bullet.h"

void Bullet::updateLocation() {
    bulletNode->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    distanceTravelled = distanceTravelled + 
        sqrt(pow(direction.x*velocity,2)+pow(direction.y*velocity,2)+pow(direction.z*velocity,2)); 
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
            distanceTravelled(0.0)
{
}

Bullet::~Bullet() {
    sceneMgr->destroySceneNode(name);
    sceneMgr->destroyRibbonTrail(rname);
}
        
