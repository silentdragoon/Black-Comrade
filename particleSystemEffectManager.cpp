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

std::string ParticleSystemEffectManager::createUnique(std::string name) {
    std::stringstream out;
    out << num++;
    name+=out.str();
    return name;
}

void ParticleSystemEffectManager::createExplosion(Vector3 pos) {
    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/awesomesauce");
    activeEffects->push_back(pse);

    nname = createUnique("enode");
    pname = createUnique("effect");
    SceneNode *effectNode1 = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode1->setPosition(pos);
    ParticleSystemEffect *pse1 = new ParticleSystemEffect(sceneMgr, effectNode1, pname, "FX/ExplosionSparks");
    activeEffects->push_back(pse1);

    nname = createUnique("enode");
    pname = createUnique("effect");
    SceneNode *effectNode2 = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode2->setPosition(pos);
    ParticleSystemEffect *pse2 = new ParticleSystemEffect(sceneMgr, effectNode2, pname, "FX/Smoke");
    activeEffects->push_back(pse2);
}

void ParticleSystemEffectManager::createSparks(Vector3 pos) {
    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/Sparks");
    activeEffects->push_back(pse);
}

void ParticleSystemEffectManager::createMuzzleFlash(Vector3 pos) {
    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/Explodder");
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