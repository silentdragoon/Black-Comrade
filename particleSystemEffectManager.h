#ifndef PARTICLESYSTEMEFFECTMANAGER_H
#define PARTICLESYSTEMEFFECTMANAGER_H

#include <OGRE/Ogre.h>
#include <string>
#include <vector>
#include <sstream>
#include "ITickable.h"
#include "particleSystemEffect.h"

using namespace Ogre;

class ParticleSystemEffectManager : public ITickable {
    private:
        std::vector<ParticleSystemEffect*> *activeEffects;
        int num;
        SceneManager *sceneMgr;

        std::string createUnique(std::string name);
    public:
        ParticleSystemEffectManager(SceneManager *sceneMgr);
        ~ParticleSystemEffectManager();

        void createExplosion(Vector3 pos);
        void createSparks(Vector3 pos);
        void createMuzzleFlash(Vector3 pos);
        void updateEffects();
        void tick();
};

#endif
