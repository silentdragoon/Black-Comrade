
#include "enemyState.h"

#include <iostream>

EnemyState::EnemyState() :
    position(new Vector3(0.0,0.0,-50.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    eSceneNode(0),
    mSceneMgr(0)
{}

EnemyState::EnemyState(SceneNode *neSceneNode, SceneManager *mSceneMgr) :
    position(new Vector3(0.0,0.0,-50.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    eSceneNode(neSceneNode),
    mSceneMgr(mSceneMgr)
{
    rRayQuery = new RayQuery( mSceneMgr );
    
    updateOgre();
}

void EnemyState::tick()
{
    if (mSceneMgr != 0) {
        Vector3 result(0,0,0);
        float dRight, dLeft, tmp;
    
        Vector3 futPos( position->x+(FVEL*LOOKA)*sin(yaw), position->y, position->z+(FVEL*LOOKA)*cos(yaw));
    
        Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
        dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);
        cout << " LD: " << dLeft;
    
        Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
        dRight = rRayQuery->RaycastFromPoint(futPos, right, result);
        cout << " RD: " << dRight << endl;
    
        tmp = (dLeft + dRight) /2 - dRight;
    
        cout << " angle: " << 1.0f/2.0f*(atan(tmp/(FVEL*LOOKA))) << endl;
    
        yaw +=   1.0f/2.0f*atan(tmp/(FVEL*LOOKA));
    
    
        position->x += FVEL * sin(yaw);
        position->z += FVEL * cos(yaw);
    }    
    updateOgre();
}

void EnemyState::updateOgre()
{
    eSceneNode->resetOrientation();

    eSceneNode->setPosition(*position);
    
    Radian roll(this->roll);
    Radian pitch(this->pitch);
    Radian yaw(this->yaw);
    
    eSceneNode->yaw(yaw);
    eSceneNode->roll(roll);
    eSceneNode->pitch(pitch);
        
}

RakNet::RakString EnemyState::GetName(void) const {return RakNet::RakString("EnemyState");}

RM3SerializationResult EnemyState::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(position->x);
    serializeParameters->outputBitstream[0].Write(position->y);
    serializeParameters->outputBitstream[0].Write(position->z);
    serializeParameters->outputBitstream[0].Write(roll);
    serializeParameters->outputBitstream[0].Write(pitch);
    serializeParameters->outputBitstream[0].Write(yaw);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void EnemyState::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(position->x);	
    deserializeParameters->serializationBitstream[0].Read(position->y);	
    deserializeParameters->serializationBitstream[0].Read(position->z);	
    deserializeParameters->serializationBitstream[0].Read(roll);
    deserializeParameters->serializationBitstream[0].Read(pitch);
    deserializeParameters->serializationBitstream[0].Read(yaw);

    //print();
}

void EnemyState::print() {
    printf("EnemyState is now (X:%.1f Y:%.1f Z:%.1f), (R:%.1f P:%.1f Y:%.1f)\n", position->x, position->y, position->z, roll, pitch, yaw);
}
