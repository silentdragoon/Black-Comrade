#include "bullet.h"

void Bullet::updateLocation() {
    node->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    distanceTravelled = distanceTravelled + 
        sqrt(pow(direction.x*velocity,2)+pow(direction.y*velocity,2)+pow(direction.z*velocity,2)); 
}

Vector3 Bullet::getDeathSpark() {
    return deathSpark;
}

Bullet::Bullet(IBulletOwner *owner, IBulletTarget *target,
               SceneNode *bulletNode, double distanceToTravel)
    : owner(owner)
    , target(target)
    , origin(owner->getBulletOrigin())
    , direction(owner->getBulletDirection())
    , node(bulletNode)
    , velocity(owner->getBulletVelocity())
    , distanceTravelled(0)
    , distanceToTravel(distanceToTravel)
    , madeNoise(false)
{}

IBulletOwner* Bullet::getOwner() { return owner; }

IBulletTarget* Bullet::getTarget() { return target; }

Bullet::Bullet(SceneNode *bulletNode,
        SceneManager *sceneMgr,
        string name,
        string rname,
        Vector3 direction,
        int velocity,
        double dtt,
        PlayerStats *mPlayerStats)
        : 
            node(bulletNode),
            sceneMgr(sceneMgr),
            name(name), 
            rname(rname),
            direction(direction),
            velocity(velocity),
            distanceToTravel(dtt),
            distanceTravelled(0.0),
            enemy(0),
            hitEnemy(false),
            hitShip(false),
            playerStats(mPlayerStats)
{
    deathSpark = Vector3(node->getPosition().x+(direction.x*dtt),node->getPosition().y+(direction.y*dtt),node->getPosition().z+(direction.z*dtt));
    origin = node->getPosition();
    madeNoise = false;
}

SceneNode *Bullet::getNode() { return node; }

Vector3 Bullet::getOrigin() { return origin; }

Bullet::~Bullet() {
    //sceneMgr->destroySceneNode(name);
    //sceneMgr->destroyRibbonTrail(rname);
}
        
