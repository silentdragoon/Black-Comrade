#include "collision.h"

Collision::Collision() {}

Collision::Collision(bool isCollided, dFloat normals[16], dFloat contacts[16], dFloat penetration[16]):
    isCollided(isCollided),
    normals(normals),
    contacts(contacts),
    penetration(penetration)
{}


