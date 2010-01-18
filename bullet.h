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
        string name;
        string rname;
        SceneManager *sceneMgr;

        Vector3 direction;
        int velocity;

        bool light;
        Vector3 lightColor;
        
        bool trail;
        Vector3 trailColor;

               
   public:
        int aliveTicks;
        Bullet(SceneNode *bulletNode,
            SceneManager *sceneMgr,
            string name,
            string rname,
            Vector3 direction, 
            int velocity,
            bool light,
            bool trail);

        ~Bullet();

        void updateLocation();
        void setLightColor(Vector3 lc);
        void setTrailColor(Vector3 tc);
};

#endif

