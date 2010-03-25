#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "ITickable.h"
#include "particleSystemEffectManager.h"

class Objective : public ITickable {
    private:
        ParticleSystemEffectManager *particleSystemEffectManager;
        int health;
        int originalHealth;
    public:
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        ~Objective();

        float getHealth();
        void damageObjective();

        void tick();
};

#endif
