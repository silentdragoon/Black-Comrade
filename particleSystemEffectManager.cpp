#include "particleSystemEffectManager.h"

ParticleSystemEffectManager::ParticleSystemEffectManager(SceneManager *sceneMgr, MapManager *mapMgr,
        SceneNode *shipSceneNode) :
    sceneMgr(sceneMgr),
    mapMgr(mapMgr),
    shipSceneNode(shipSceneNode),
    num(0)
{
    activeEffects = new std::vector<ParticleSystemEffect*>();
}

ParticleSystemEffectManager::~ParticleSystemEffectManager() {
    delete activeEffects;
}

void ParticleSystemEffectManager::createEffect(EffectType type, Vector3 position) {
    switch (type) {
        case ET_SPARK:
            createSparks(position);
            break;
    }
}

std::string ParticleSystemEffectManager::createUnique(std::string name) {
    std::stringstream out;
    out << num++;
    name+=out.str();
    return name;
}

void ParticleSystemEffectManager::makeObjective() {
    Vector3 pos = mapMgr->getObjectivePosition();

    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");
    std::string pname2 = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    Light *rl = sceneMgr->createLight("reactorLight");
    rl->setType(Light::LT_POINT);
    rl->setDiffuseColour(0.6,0.2,0.8);
    rl->setSpecularColour(0.8,0.4,1.0);
    rl->setAttenuation(600, 0, 1, 0);
    rl->setPowerScale(5.0);
    effectNode->attachObject(rl);
    obj1 = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/reactor");
    obj2 = new ParticleSystemEffect(sceneMgr, effectNode, pname2, "FX/reactor2");
}

void ParticleSystemEffectManager::destroyObjective() {
    Vector3 pos = mapMgr->getObjectivePosition();

    obj1->stopEmitting();
    obj2->stopEmitting();

    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/reactorExplosion");
    activeEffects->push_back(pse);

    nname = createUnique("enode");
    pname = createUnique("effect");

    SceneNode *effectNode2 = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode2->setPosition(pos);
    ParticleSystemEffect *pse2 = new ParticleSystemEffect(sceneMgr, effectNode2, pname, "FX/reactorExplosion2");
    activeEffects->push_back(pse2);
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
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/MultiSparks");
    activeEffects->push_back(pse);

    nname = createUnique("enode");
    pname = createUnique("effect");

    SceneNode *effectNode1 = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode1->setPosition(pos);
    ParticleSystemEffect *pse1 = new ParticleSystemEffect(sceneMgr, effectNode1, pname, "FX/Sparks");
    activeEffects->push_back(pse1);
}

void ParticleSystemEffectManager::createMuzzleFlash(Vector3 pos) {
    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");

    SceneNode *effectNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nname);
    effectNode->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, effectNode, pname, "FX/Explodder");
    activeEffects->push_back(pse);
}

void ParticleSystemEffectManager::createEngineGlow(Vector3 pos) {
    // WWAARRGARRBBLLL
    std::string nname = createUnique("enode");
    std::string pname = createUnique("effect");
    SceneNode *engine = shipSceneNode->createChildSceneNode();
    engine->setPosition(pos);
    ParticleSystemEffect *pse = new ParticleSystemEffect(sceneMgr, engine, pname, "FX/engines");
    Light *engLight = sceneMgr->createLight();
    engLight->setType(Light::LT_POINT);
    engLight->setDiffuseColour(0.2,0.2,1.0);
    engLight->setSpecularColour(0.2,0.2,1.0);
    engLight->setAttenuation(500,0,1,1);
    engine->attachObject(engLight);
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
