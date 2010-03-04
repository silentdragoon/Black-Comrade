#ifndef PARTICLESYSTEMEFFECT_H
#define PARTICLESYSTEMEFFECT_H

#include <OGRE/Ogre.h>
#include <string>

using namespace Ogre;

class ParticleSystemEffect {
    private:
        int ticksToLive;
        SceneNode *node;
        SceneManager *sceneMgr;
        ParticleSystem *particleSystem;

    public:
        ParticleSystemEffect(SceneManager *sceneMgr, SceneNode *effect, std::string pName, std::string eName);
        ~ParticleSystemEffect();

        void decreaseTicks();
        int getTicks();
};

#endif
