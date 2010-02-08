#include "enemy.h"

Enemy::Enemy(int health, int id) :
    health(health),
    fire(false),
    fireDelay(0),
    id(id),
    position(new Vector3())
{}

Enemy::~Enemy()
{}

int Enemy::getHealth() {
    return health;
}

void Enemy::setPosition(Vector3 v)
{
	position->x = v.x;
	position->y = v.y;
	position->z = v.z;
}

Vector3* Enemy::getPosition() { return position; }

Vector3* Enemy::getOrientation() { return new Vector3(pitch,yaw,roll); }

std::string Enemy::getMeshName() { return "squid.mesh"; }

IDrawable* Enemy::getParentObject() { return NULL; }

void Enemy::setOrientation(Real mRoll, Real mPitch, Real mYaw)
{
	roll = mRoll;
    pitch = mPitch;
    yaw = mYaw;
}

Real Enemy::getRoll() { return roll; }
Real Enemy::getPitch() { return pitch; }
Real Enemy::getYaw() { return yaw; }

Vector3 Enemy::getDirection()
{
	return SceneNodeManager::rollPitchYawToDirection(roll,pitch,yaw);
}
