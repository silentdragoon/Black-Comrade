#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "ITickable.h"
#include "particleSystemEffectManager.h"

class Objective : public ITickable {
    private:
        ParticleSystemEffectManager *particleSystemEffectManager;
        int health;
    public:
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        ~Objective();

        int getHealth();
        void damageObjective();

        void tick();
};

#endif
