
#include "connectionPiece.h"

ConnectionPiece::ConnectionPiece(Vector3 position, float yaw)
    : position(position)
    , orientation(Vector3(0,yaw,0))
    , meshOrientation(Vector3(0,0,0))
{}

Vector3 *ConnectionPiece::getPosition()
{
    return &position;
}

Vector3 *ConnectionPiece::getOrientation()
{
    return &orientation;
}

Vector3 *ConnectionPiece::getMeshOrientation()
{
    return &meshOrientation;
}

IDrawable *ConnectionPiece::getParentObject()
{
    return NULL;
}

std::string ConnectionPiece::getMeshName()
{
    return std::string("newConnExport.mesh");
}

void ConnectionPiece::tick()
{
    ;
}

