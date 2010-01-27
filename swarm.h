#ifndef _SWARM_H 
#define _SWARM_H

#include <OGRE/Ogre.h>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "enemy.h"

using namespace Ogre;
using namespace std;

class Swarm
{
    private:
        SceneManager *sceneMgr;
        SceneNode *leadSN;
        vector<Enemy*> members;
        int size;
        int id;
        Vector3 location;
    public:

        Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr);
        ~Swarm();

        Vector3 getAverageAlignment();
        Vector3 getAveragePosition();

        void updateLocation();
};

#endif
