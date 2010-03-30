#include "bullet.h"

Bullet::Bullet(IBulletOwner *owner, IBulletTarget *target,
               SceneManager *sceneMgr, double distanceToTravel)
    : owner(owner)
    , target(target)
    , damage(owner->getBulletDamage())
    , origin(owner->getBulletOrigin())
    , direction(owner->getBulletDirection())
    , sceneMgr(sceneMgr)
    , velocity(owner->getBulletVelocity())
    , distanceTravelled(0)
    , distanceToTravel(distanceToTravel)
    , madeNoise(false)
{
    makeNode();
    pointOfImpact = Vector3(node->getPosition().x+(direction.x*distanceToTravel),
                            node->getPosition().y+(direction.y*distanceToTravel),
                            node->getPosition().z+(direction.z*distanceToTravel));
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

Vector3 Bullet::getOrigin() { return origin; }

int Bullet::getDamage() { return damage; }

void Bullet::updateLocation() {
    node->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    distanceTravelled = distanceTravelled + 
        sqrt(pow(direction.x*velocity,2)+pow(direction.y*velocity,2)+pow(direction.z*velocity,2)); 
}

Vector3 Bullet::getPointOfImpact() {
    return pointOfImpact;
}

Bullet::~Bullet() {
    sceneMgr->destroySceneNode(node);
    sceneMgr->destroyRibbonTrail(trail);
}
        
