#include "enemy.h"

Enemy::Enemy(int health, int id) :
    health(health),
    fire(false),
    fireDelay(0),
    position(new Vector3())
{}

Enemy::~Enemy()
{}

Vector3 Enemy::getDirection() {
    Vector3 direction;
    direction.x = sin(yaw) + cos(roll);
    direction.y = sin(roll) + cos(pitch);
    direction.z = sin(pitch) + cos(yaw);
    
    return direction;
}

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

RakNet::RakString Enemy::GetName(void) const {return RakNet::RakString("Enemy");}

RM3SerializationResult Enemy::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(getPosition()->x);
    serializeParameters->outputBitstream[0].Write(getPosition()->y);
    serializeParameters->outputBitstream[0].Write(getPosition()->z);
    serializeParameters->outputBitstream[0].Write(roll);
    serializeParameters->outputBitstream[0].Write(pitch);
    serializeParameters->outputBitstream[0].Write(yaw);
    serializeParameters->outputBitstream[0].Write(health);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void Enemy::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(position->x);	
    deserializeParameters->serializationBitstream[0].Read(position->y);	
    deserializeParameters->serializationBitstream[0].Read(position->z);	
    deserializeParameters->serializationBitstream[0].Read(roll);
    deserializeParameters->serializationBitstream[0].Read(pitch);
    deserializeParameters->serializationBitstream[0].Read(yaw);
    deserializeParameters->serializationBitstream[0].Read(health);
}
