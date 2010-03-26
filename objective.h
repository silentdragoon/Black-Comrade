#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "ITickable.h"
#include "particleSystemEffectManager.h"
#include "constManager.h"
#include "replicaObject.h"

class Objective : public ITickable, public ReplicaObject {
    private:
        ParticleSystemEffectManager *particleSystemEffectManager;
        int health;
        int escapeTime;
        int ticks;
        bool boom;
        int originalHealth;
    public:
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        Objective();
        ~Objective();

        void setParticleSystemEffectManager(ParticleSystemEffectManager *particleSystemEffectManager);

        float getHealth();
        void damageObjective();
        int getEscapeTime();

        void tick();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
