
#include "fan.h"

Fan::Fan(Vector3 position, float yaw, IDrawable *parent)
    : position(position)
    , orientation(Vector3(0,yaw,0))
    , meshOrientation(Vector3(0,0,0))
    , parent(parent)
{}

Vector3 *Fan::getPosition()
{
    return &position;
}

Vector3 *Fan::getOrientation()
{
    return &orientation;
}

Vector3 *Fan::getMeshOrientation()
{
    return &meshOrientation;
}

IDrawable *Fan::getParentObject()
{
    return parent;
}

std::string Fan::getMeshName()
{
    return std::string("fanBlades.mesh");
}

void Fan::tick()
{
    orientation.z = orientation.z + 0.1;
}

