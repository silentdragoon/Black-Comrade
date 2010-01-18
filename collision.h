#ifndef COLLISION_H
#define COLLISION_H

// Includes all Ogre classes 
#include <OGRE/Ogre.h>
#include <Newton.h>

using namespace Ogre;
using namespace std;

class Collision{
private:

public:
    bool isCollided;
    dFloat *normals;
    dFloat *contacts;
    dFloat *penetration;
    
    Collision();    
    Collision(bool isCollided, dFloat normals[16], dFloat contacts[16], dFloat penetration[16]); 
};

#endif
