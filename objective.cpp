#include "objective.h"

Objective::Objective(ParticleSystemEffectManager *particleSystemEffectManager) :
    particleSystemEffectManager(particleSystemEffectManager),
    health(10)
{}

Objective::~Objective() {}

int Objective::getHealth() { return health; }

void Objective::damageObjective() {
    if(health!=0) health--;
}

void Objective::tick() {
    if(health<=0) {
        // Destroy objective and make a giant explosion
        particleSystemEffectManager->destroyObjective();
    }
}
