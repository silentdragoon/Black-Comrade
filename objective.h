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
    public:
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        ~Objective();

        int getHealth();
        void damageObjective();
        int getEscapeTime();

        void tick();
};

#endif
