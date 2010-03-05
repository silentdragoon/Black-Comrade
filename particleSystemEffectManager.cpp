#include "particleSystemEffectManager.h"

ParticleSystemEffectManager::ParticleSystemEffectManager(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    num(0)
{
    activeEffects = new std::vector<ParticleSystemEffect*>(); 
}

ParticleSystemEffectManager::~ParticleSystemEffectManager() {
    delete activeEffects;
}

void ParticleSystemEffectManager::createExplosion(Vector3 pos) {
    std::string nname = "enode";
    std::string pname = "effect";
    std::stringstream out;
    out << num++;
    nname += out.str();
    pname += out.str();

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/Explosion");
    activeEffects->push_back(pse);
}

void ParticleSystemEffectManager::createSparks(Vector3 pos) {
    std::string nname = "enode";
    std::string pname = "effect";
    std::stringstream out;
    out << num++;
    nname += out.str();
    pname += out.str();

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/Sparks");
    activeEffects->push_back(pse);
}

void ParticleSystemEffectManager::updateEffects() {
    for(int i=0;i<activeEffects->size();i++) {
        ParticleSystemEffect *pse = activeEffects->at(i);
        if(pse->getTicks()==0) {
            delete pse;
            activeEffects->erase(activeEffects->begin()+(i));
        } else {
            pse->decreaseTicks();
        }
    }
}

void ParticleSystemEffectManager::tick() {
    updateEffects();
}
