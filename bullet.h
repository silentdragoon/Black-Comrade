#ifndef _BULLET_H 
#define _BULLET_H

#include <OGRE/Ogre.h>
#include <string>
#include <iostream>

using namespace Ogre;
using namespace std;

class Bullet
{
    private:
        SceneNode *bulletNode;

        Vector3 direction;
        int velocity;

        bool light;
        Vector3 lightColor;
        
        bool trail;
        Vector3 trailColor;

        int aliveTicks;        
   public:
        Bullet(SceneNode *bulletNode,
            Vector3 direction, 
            int velocity,
            bool light,
            bool trail);

        // TODO: Add destructor

        void updateLocation();
        void setLightColor(Vector3 lc);
        void setTrailColor(Vector3 tc);
};

#endif

