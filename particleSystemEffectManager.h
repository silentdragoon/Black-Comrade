#ifndef PARTICLESYSTEMEFFECTMANAGER_H
#define PARTICLESYSTEMEFFECTMANAGER_H

#include <OGRE/Ogre.h>
#include <string>
#include <vector>
#include <sstream>
#include "ITickable.h"
#include "particleSystemEffect.h"
#include "mapManager2.h"
#include "effectType.h"

using namespace Ogre;

class ParticleSystemEffectManager : public ITickable {
    private:
        std::vector<ParticleSystemEffect*> *activeEffects;
        int num;
        SceneManager *sceneMgr;
        MapManager *mapMgr;
        SceneNode *shipSceneNode;

        ParticleSystemEffect *obj1;
        ParticleSystemEffect *obj2;

        std::string createUnique(std::string name);
    public:
        ParticleSystemEffectManager(SceneManager *sceneMgr, MapManager *mapMgr, SceneNode *shipSceneNode);
        ~ParticleSystemEffectManager();

        void createEffect(EffectType type, Vector3 position);
        void makeObjective();
        void destroyObjective();
        void createExplosion(Vector3 pos);
        void createSparks(Vector3 pos);
        void createMuzzleFlash(Vector3 pos);
        void createEngineGlow();
        void updateEffects();
        void tick();
};

#endif
