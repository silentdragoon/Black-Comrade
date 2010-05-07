
#include "shipState.h"

#include <iostream>

ShipState::ShipState() 
    : position(new Vector3(1400.0,0.0,100.0))
{
    roll=0.0;
    pitch=0.0;
    yaw=0.0;
    speed=0.0;
}

Vector3 *ShipState::getPosition() {
    return position;
}

void ShipState::tick() {}

IDrawable* ShipState::getParentObject() { return NULL; }

Vector3* ShipState::getOrientation() { return new Vector3(pitch,yaw,roll); }

Vector3 *ShipState::getMeshOrientation() { return new Vector3(0,PI,0); }

std::string ShipState::getMeshName() { return "lolhawk.mesh"; }

EntityType ShipState::getEntityType() { return ENTT_SHIP; }

EffectType ShipState::getHitEffectType() { return ET_NONE; }

void ShipState::setDamageState(DamageState *mDamageState) {
    damageState = mDamageState;
}

void ShipState::damage(float amount) {
    damageState->damage(amount);
}

void ShipState::setX(double newX) {
    position->x = newX;
}

void ShipState::setY(double newY) {
    position->y = newY;
}

void ShipState::setZ(double newZ) {
    position->z = newZ;
}

void ShipState::setSpeed(double s) {
    speed = s;
}

double ShipState::getX() {
    return position->x;
}

double ShipState::getY() {
    return position->y;
}

double ShipState::getZ() {
    return position->z;
}

double ShipState::getSpeed() {
    return speed;
}

RakNet::RakString ShipState::GetName(void) const {return RakNet::RakString("ShipState");}

RM3SerializationResult ShipState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(position->x);
    serializeParameters->outputBitstream[0].Write(position->y);
    serializeParameters->outputBitstream[0].Write(position->z);
    serializeParameters->outputBitstream[0].Write(roll);
    serializeParameters->outputBitstream[0].Write(pitch);
    serializeParameters->outputBitstream[0].Write(yaw);
    serializeParameters->outputBitstream[0].Write(speed);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void ShipState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(position->x);	
    deserializeParameters->serializationBitstream[0].Read(position->y);	
    deserializeParameters->serializationBitstream[0].Read(position->z);	
    deserializeParameters->serializationBitstream[0].Read(roll);
    deserializeParameters->serializationBitstream[0].Read(pitch);
    deserializeParameters->serializationBitstream[0].Read(yaw);
    deserializeParameters->serializationBitstream[0].Read(speed);

    //print();
}

void ShipState::print() {
    printf("ShipState is now (X:%.1f Y:%.1f Z:%.1f), (R:%.1f P:%.1f Y:%.1f)\n", position->x, position->y, position->z, roll, pitch, yaw);
}



