#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "ITickable.h"
#include "particleSystemEffectManager.h"
#include "constManager.h"

class Objective : public ITickable {
    private:
        ParticleSystemEffectManager *particleSystemEffectManager;
        int health;
        int escapeTime;
        int ticks;
        bool boom;
        int originalHealth;
    public:
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        ~Objective();

        float getHealth();
        void damageObjective();
        int getEscapeTime();

        void tick();
};

#endif
