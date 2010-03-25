#include "objective.h"

Objective::Objective(ParticleSystemEffectManager *particleSystemEffectManager) :
    particleSystemEffectManager(particleSystemEffectManager),
    health(10),
    escapeTime(ConstManager::getInt("escape_time")),
    ticks(0),
    boom(false),
    originalHealth(10)
{}

Objective::~Objective() {}

float Objective::getHealth() { return (float)health/(float)originalHealth; }

void Objective::damageObjective() {
    if(health!=0) health--;
}

int Objective::getEscapeTime() { return escapeTime; }

void Objective::tick() {
    if(health<=0) {
        if(!boom) {
            // Destroy objective and make a giant explosion
            particleSystemEffectManager->destroyObjective();
            boom = true;
        }
        // Count down timer
        ticks++;
        if(ticks>=1/ConstManager::getFloat("tick_period")) {
            ticks=0;
            escapeTime--;
            std::cout << "ESCAPE: " << escapeTime << " seconds" << std::endl;
        }
    }
}
