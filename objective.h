#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include "ITickable.h"
#include "particleSystemEffectManager.h"
#include "constManager.h"
#include "replicaObject.h"

#include "IBulletTarget.h"

class Objective : public ITickable, public ReplicaObject,
                  public IBulletTarget {
    private:
        ParticleSystemEffectManager *particleSystemEffectManager;
        //CollisionManager *collisionMgr;
        int health;
        int escapeTime;
        int ticks;
        bool boom;
        int originalHealth;
        Vector3 position;
    public:
        //Objective(ParticleSystemEffectManager *particleSystemEffectManager, CollisionManager *colMgr);
        Objective(ParticleSystemEffectManager *particleSystemEffectManager);
        Objective();
        ~Objective();

        void setParticleSystemEffectManager(ParticleSystemEffectManager *particleSystemEffectManager);

        EntityType getEntityType();
        float getOriginalHealth();
        float getHealth();
        float getHealthPercentage();
        void damage(float amount);
        EffectType getHitEffectType();

        void damageObjective();
        int getEscapeTime();
        Vector3 getPosition();
        void setPosition(Vector3 pos);

        void tick();

        virtual RakNet::RakString GetName(void) const;
        virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
        virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif
