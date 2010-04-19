
#include "door.h"

Door::Door(Vector3 position, float yaw)
    : startPosition(position)
    , orientation(Vector3(0,yaw,0))
    , meshOrientation(Vector3(0,0,0))
    , fracOpen(0.0)
    , motion(0)
{}
        
Vector3 *Door::getPosition()
{
    position = startPosition + fracOpen * Vector3(0,
        ConstManager::getFloat("tunnel_height"),0);
    
    return &position;
}

Vector3 *Door::getOrientation()
{
    return &orientation;
}

Vector3 *Door::getMeshOrientation()
{
    return &meshOrientation;
}

IDrawable *Door::getParentObject()
{
    return NULL;
}

std::string Door::getMeshName()
{
    return std::string("door.mesh");
}

bool Door::isOpen()
{
    return fracOpen = 1;
}

bool Door::isClosed()
{
    return fracOpen = 0;
}

float Door::moveSpeed()
{
    return ConstManager::getFloat("tick_period")
        / ConstManager::getFloat("door_open_time");
}

void Door::open()
{
    motion = moveSpeed();
    //cout << "Motion: " << motion << endl;
}

void Door::close()
{
    motion = - moveSpeed();
}

void Door::tick()
{
    if(fracOpen >= 1) {
        fracOpen = 1;
    }
    
    if(fracOpen <= 0) {
        fracOpen = 0;
    }
    
    fracOpen += motion;
    
    
}

