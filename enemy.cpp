#include "enemy.h"

Enemy::Enemy(SceneNode *node, int health) :
    node(node),
    health(health)
{}

Enemy::~Enemy()
{}

Vector3 Enemy::getDirection() {
    Quaternion orient = node->getOrientation();
    return orient.zAxis();
}

Vector3 Enemy::getLocation() {
    return node->getPosition();
}

int Enemy::getHealth() {
    return health;
}

void Enemy::setLocation(Vector3 v)
{
	node->setPosition(v);
}

void Enemy::setOrientation(Real roll, Real pitch, Real yaw)
{
	Radian troll(roll);
    Radian tpitch(pitch);
    Radian tyaw(yaw);
    
    node->yaw(tyaw);
    node->roll(troll);
    node->pitch(tpitch);
}
