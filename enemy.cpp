
#include "enemy.h"

Enemy::Enemy(int health, int id) :
    health(health),
    isReplicated(false),
    fire(false),
    fireDelay(0),
    position(new Vector3()),
    yaw(0),
    pitch(0),
    roll(0),
    isDead(false)
{}

Enemy::Enemy() :
    isReplicated(false),
    health(1),
    fire(false),
    fireDelay(0),
    position(new Vector3()),
    id(-1),
    yaw(0),
    pitch(0),
    roll(0),
    isDead(false)
{}

Enemy::~Enemy()
{}

Vector3 Enemy::getDirection() {
	return SceneNodeManager::rollPitchYawToDirection(roll,pitch,yaw);
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

Vector3 *Enemy::getMeshOrientation() { return new Vector3(0,PI,0); }

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
    
    serializeParameters->outputBitstream[0].Write(position->x);
    serializeParameters->outputBitstream[0].Write(position->y);
    serializeParameters->outputBitstream[0].Write(position->z);
    serializeParameters->outputBitstream[0].Write(roll);
    serializeParameters->outputBitstream[0].Write(pitch);
    serializeParameters->outputBitstream[0].Write(yaw);
    serializeParameters->outputBitstream[0].Write(health);
    serializeParameters->outputBitstream[0].Write(fire);
    serializeParameters->outputBitstream[0].Write(isDead);


    return RM3SR_BROADCAST_IDENTICALLY;
}

void Enemy::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    bool fire2 = false;
    deserializeParameters->serializationBitstream[0].Read(position->x);	
    deserializeParameters->serializationBitstream[0].Read(position->y);	
    deserializeParameters->serializationBitstream[0].Read(position->z);	
    deserializeParameters->serializationBitstream[0].Read(roll);
    deserializeParameters->serializationBitstream[0].Read(pitch);
    deserializeParameters->serializationBitstream[0].Read(yaw);
    deserializeParameters->serializationBitstream[0].Read(health);
    deserializeParameters->serializationBitstream[0].Read(fire2);
    deserializeParameters->serializationBitstream[0].Read(isDead);
    if (fire2) fire = true;
}
