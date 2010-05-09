
#include "Fan.h"

Fan::Fan(Vector3 position, float yaw)
    : startPosition(position)
    , orientation(Vector3(0,yaw,0))
    , meshOrientation(Vector3(0,0,0))
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
    return NULL;
}

std::string Fan::getMeshName()
{
    return std::string("fanBlades.mesh");
}

void Fan::tick()
{
    ;
}

