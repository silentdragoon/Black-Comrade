#include "waypoint.h"

Waypoint::Waypoint(string *fu, int x, int y) :
    name(fu),
    x(x),
    y(y)
{}

Waypoint::~Waypoint()
{}

string* Waypoint::getName()
{
    return name;
}

int Waypoint::getX()
{
    return x;
}

int Waypoint::getY()
{
    return y;
}

