#include "bullet.h"

Bullet::Bullet(IBulletOwner *owner, IBulletTarget *target,
               SceneManager *sceneMgr, double distanceToTravel)
    : owner(owner)
    , target(target)
    , origin(owner->getBulletOrigin())
    , direction(owner->getBulletDirection())
    , sceneMgr(sceneMgr)
    , velocity(owner->getBulletVelocity())
    , distanceTravelled(0)
    , distanceToTravel(distanceToTravel)
    , madeNoise(false)
{
//deathSpark(Vector3(node->getPosition().x+(direction.x*dtt),node->getPosition().y+(direction.y*dtt),node->getPosition().z+(direction.z*dtt));
    makeNode();
}

void Bullet::makeNode() {

    node = sceneMgr->getRootSceneNode()->createChildSceneNode();
    node->setPosition(owner->getBulletOrigin());

    BillboardSet *bbbs = sceneMgr->createBillboardSet(1);
    bbbs->setMaterialName("PE/Streak");
    Billboard *bbb = bbbs->createBillboard(0,0,0,owner->getBulletColour());
    bbb->setDimensions(0.7,0.7);

    trail = sceneMgr->createRibbonTrail();
    trail->setMaterialName("PE/LightRibbonTrail");
    trail->setTrailLength(75);
    trail->setMaxChainElements(400);
    trail->setInitialColour(0,owner->getBulletColour());
    trail->setInitialWidth(0,0.7);

    trail->addNode(node);

    sceneMgr->getRootSceneNode()->attachObject(trail);

    Light *l = sceneMgr->createLight();
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(owner->getBulletColour());
    l->setSpecularColour(owner->getBulletColour());
    l->setAttenuation(100,0.5,0.0005,0);

    node->attachObject(bbbs);
    node->attachObject(l);
}

IBulletOwner* Bullet::getOwner() { return owner; }

IBulletTarget* Bullet::getTarget() { return target; }

SceneNode *Bullet::getNode() { return node; }

Vector3 Bullet::getOrigin() { return origin; }

void Bullet::updateLocation() {
    node->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    distanceTravelled = distanceTravelled + 
        sqrt(pow(direction.x*velocity,2)+pow(direction.y*velocity,2)+pow(direction.z*velocity,2)); 
}

Vector3 Bullet::getDeathSpark() {
    return deathSpark;
}

Bullet::~Bullet() {
    sceneMgr->destroySceneNode(node);
    sceneMgr->destroyRibbonTrail(trail);
}
        
