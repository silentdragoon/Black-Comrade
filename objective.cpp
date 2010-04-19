#include "objective.h"

Objective::Objective(ParticleSystemEffectManager *particleSystemEffectManager, CollisionManager *colMgr) :
    particleSystemEffectManager(particleSystemEffectManager),
    collisionMgr(colMgr),
    health(10),
    escapeTime(ConstManager::getInt("escape_time")),
    ticks(0),
    boom(false),
    originalHealth(10)
{}

Objective::Objective()
    : health(10)
    , escapeTime(ConstManager::getInt("escape_time"))
    , boom(false)
    , ticks(0)
    , originalHealth(10)
{}

void Objective::setParticleSystemEffectManager(ParticleSystemEffectManager *psem) {
    particleSystemEffectManager = psem;
}

Objective::~Objective() {}

EntityType Objective::getEntityType() { return ENTT_OBJECTIVE; }

float Objective::getHealthPercentage() { return (float)health/(float)originalHealth; }

float Objective::getHealth() { return (float) health; }

EffectType Objective::getHitEffectType() { return ET_NONE; }

void Objective::damageObjective() {
    if(health!=0) health--;
}

void Objective::damage(float amount) {
    health = health - amount;
} 

int Objective::getEscapeTime() { return escapeTime; }

void Objective::tick() {
    if(health<=0) {
        if(!boom) {
            // Destroy objective and make a giant explosion
            particleSystemEffectManager->destroyObjective();
            collisionManager->removeObjective();
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

RakNet::RakString Objective::GetName(void) const {return RakNet::RakString("Objective");}

RM3SerializationResult Objective::Serialize(SerializeParameters *serializeParameters) {
    serializeParameters->outputBitstream[0].Write(health);
    serializeParameters->outputBitstream[0].Write(escapeTime);

    return RM3SR_BROADCAST_IDENTICALLY;
}

void Objective::Deserialize(RakNet::DeserializeParameters *deserializeParameters) {
    deserializeParameters->serializationBitstream[0].Read(health);	
    deserializeParameters->serializationBitstream[0].Read(escapeTime);	

}
