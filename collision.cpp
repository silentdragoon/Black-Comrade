#include "collision.h"

Collision::Collision() {}

Collision::Collision(bool isCollided, dFloat _normals[16], dFloat _contacts[16], dFloat _penetration[16])
    :
    isCollided(isCollided),
    normals(_normals),
    contacts(_contacts),
    penetration(_penetration)
{}


