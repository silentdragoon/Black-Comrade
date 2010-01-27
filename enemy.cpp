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
