#include "particleSystemEffect.h"

ParticleSystemEffect::ParticleSystemEffect(SceneManager *sceneMgr, SceneNode *effect, std::string pName, std::string eName) :
    sceneMgr(sceneMgr),
    node(effect)
{
    particleSystem = sceneMgr->createParticleSystem(pName,eName);
    particleSystem->setSpeedFactor(100.0f);
    //particleSystem->createParticle();
    node->attachObject(particleSystem);
    ticksToLive = (int)particleSystem->getEmitter(0)->getDuration() + (int)particleSystem->getEmitter(0)->getTimeToLive();
}

ParticleSystemEffect::~ParticleSystemEffect() {
    node->detachObject(particleSystem);
    sceneMgr->destroyParticleSystem(particleSystem);
    sceneMgr->destroySceneNode(node);
}

void ParticleSystemEffect::decreaseTicks() {
    ticksToLive--;
}

int ParticleSystemEffect::getTicks() {
    return ticksToLive;
}
