
#include "shipState.h"

#include <iostream>

ShipState::ShipState() {
    flying=0;
    position = new Vector3(1400.0,0.0,100.0);
    roll=0.0;
    pitch=0.0;
    yaw=0.0;
}

ShipState::ShipState(SceneNode *shipSceneNode, Flying *flying ) :
    position(new Vector3(0.0,0.0,-500.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    shipSceneNode(shipSceneNode),
    flying(flying)
{}

void ShipState::tick() {

    if (flying != 0) {
        position->x = flying->position->x;
        position->y = flying->position->y;
        position->z = flying->position->z;

        roll = flying->roll;
        pitch = flying->pitch;
        yaw = flying->yaw;
    }
    updateOgre();
}

void ShipState::updateOgre() {
    
	shipSceneNode->resetOrientation();

    shipSceneNode->setPosition(*position);
    
    Radian roll(this->roll);
    Radian pitch(this->pitch);
    Radian yaw(this->yaw);
    
    shipSceneNode->yaw(yaw);
    shipSceneNode->roll(roll);
    shipSceneNode->pitch(pitch);
        
    //std::cout << shipSceneNode->getPosition().z << std::endl;
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

double ShipState::getX() {
    return position->x;
}

double ShipState::getY() {
    return position->y;
}

double ShipState::getZ() {
    return position->z;
}

RakNet::RakString ShipState::GetName(void) const {return RakNet::RakString("ShipState");}

RM3SerializationResult ShipState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(position->x);
    serializeParameters->outputBitstream[0].Write(position->y);
    serializeParameters->outputBitstream[0].Write(position->z);
    serializeParameters->outputBitstream[0].Write(roll);
    serializeParameters->outputBitstream[0].Write(pitch);
    serializeParameters->outputBitstream[0].Write(yaw);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void ShipState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(position->x);	
    deserializeParameters->serializationBitstream[0].Read(position->y);	
    deserializeParameters->serializationBitstream[0].Read(position->z);	
    deserializeParameters->serializationBitstream[0].Read(roll);
    deserializeParameters->serializationBitstream[0].Read(pitch);
    deserializeParameters->serializationBitstream[0].Read(yaw);

    //print();
}

void ShipState::print() {
    printf("ShipState is now (X:%.1f Y:%.1f Z:%.1f), (R:%.1f P:%.1f Y:%.1f)\n", position->x, position->y, position->z, roll, pitch, yaw);
}



