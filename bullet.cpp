#include "bullet.h"

void Bullet::updateLocation() {
    bulletNode->translate((direction.x)*velocity,(direction.y)*velocity,(direction.z)*velocity);

    aliveTicks++;
}

void Bullet::setLightColor(Vector3 lc) {
    lightColor = lc;
}

void Bullet::setTrailColor(Vector3 tc) {
    trailColor = tc;
}

Bullet::Bullet(SceneNode *bulletNode,
        SceneManager *sceneMgr,
        string name,
        string rname,
        Vector3 direction,
        int velocity,
        bool light,
        bool trail)
        : 
            bulletNode(bulletNode),
            sceneMgr(sceneMgr),
            name(name), 
            rname(rname),
            direction(direction),
            velocity(velocity),
            light(light),
            trail(trail),
            aliveTicks(0)
{
}

Bullet::~Bullet() {
    sceneMgr->destroySceneNode(name);
    sceneMgr->destroyRibbonTrail(rname);
}
        
