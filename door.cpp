
#include "door.h"

Door::Door(Vector3 position, float yaw)
    : position(position)
    , orientation(Vector3(0,yaw,0))
    , meshOrientation(Vector3())
    {}
        
Vector3 *Door::getPosition()
{
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
