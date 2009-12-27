#include "bullet.h"

void Bullet::updateLocation() {
    // Assuming direction is some sort of unit vector or something
    Vector3 currentPos = bulletNode->getPosition();
    cout << currentPos.x << " " << currentPos.y << " " << currentPos.z << endl;
    //bulletNode->setPosition(Vector3(currentPos.x+((direction.x)*velocity),currentPos.y+((direction.y)*velocity),currentPos.z+((direction.z)*velocity)));
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
        Vector3 direction,
        int velocity,
        bool light,
        bool trail)
        : 
            bulletNode(bulletNode),
            direction(direction),
            velocity(velocity),
            light(light),
            trail(trail),
            aliveTicks(0)
{
}

Bullet::~Bullet() {
    delete bulletNode;
}
        
