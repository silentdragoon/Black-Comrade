#include "enemy.h"

Enemy::Enemy(SceneNode *node, int health, SceneManager *sceneMgr) :
    node(node),
    health(health),
    sceneMgr(sceneMgr),
    fire(false),
    fireDelay(0)
{}

Enemy::~Enemy()
{
    sceneMgr->destroySceneNode(node);
}

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

Entity* Enemy::getEntity()
{
	return (Entity*)node->getAttachedObject(0);
}

void Enemy::setOrientation(Real roll, Real pitch, Real yaw)
{
	node->resetOrientation();

	yaw += 3.14159;

	Radian troll(roll);
    Radian tpitch(pitch);
    Radian tyaw(yaw);
    
    
    
    node->yaw(tyaw);
    node->roll(troll);
    node->pitch(tpitch);
}
