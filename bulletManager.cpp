#include "bulletManager.h"
#include "const.h"

BulletManager::BulletManager(ShipState *shipState, SceneManager *sceneMgr,
                GunState *pilotGunState, GunState *engineerGunState,
                GunState *navigatorGunState, CollisionManager *colMgr,
                SwarmManager *swarmMgr, SceneNodeManager *sceneNodeMgr,
                DamageState *damageState, ParticleSystemEffectManager *particleSystemEffectManager,
                Objective *objective)
    : shipState(shipState)
    , sceneMgr(sceneMgr)
    , pilotGunState(pilotGunState)
    , engineerGunState(engineerGunState)
    , navigatorGunState(navigatorGunState)
    , colMgr(colMgr)
    , swarmMgr(swarmMgr)
    , sceneNodeMgr(sceneNodeMgr)
    , damageState(damageState)
    , particleSystemEffectManager(particleSystemEffectManager)
    , objective(objective)
{
    activeBullets = new std::vector<Bullet*>();
    colMgr->addMesh( sceneNodeMgr->getEntity(shipState) );
}

std::vector<Bullet*> * BulletManager::getActiveBullets() {
    return activeBullets;
}

void BulletManager::tick()
{
    updateBullets();

    // Guns shoot if neccessary
    handleGun(pilotGunState);
    handleGun(engineerGunState);
    handleGun(navigatorGunState);
    
    // Enemies shoot if neccessary
    handleEnemies(swarmMgr->getAllLocalEnemies());
    handleEnemies(swarmMgr->getReplicatedEnemies());
}

void BulletManager::fire(IBulletOwner *owner) {
    IBulletTarget *target;
    double distanceToTravel = findTarget(owner,&target);
    Bullet *b = new Bullet(owner, target, sceneMgr, distanceToTravel);
    activeBullets->push_back(b);
    particleSystemEffectManager->createEffect(owner->getMuzzleFlashEffectType(), owner->getBulletOrigin());
}

double BulletManager::getDistanceTo(IBulletTarget *possibleTarget,IBulletOwner *owner) {
    double distance = 0.0;
    Vector3 origin = owner->getBulletOrigin();
    Vector3 direction = owner->getBulletDirection();

    switch (possibleTarget->getEntityType()) {
        case ENTT_OBJECTIVE:
            distance = colMgr->getRCObjDist(&origin,&direction);
            break;
        case ENTT_MAP:
            distance = colMgr->getRCMapAndMovObjsDist(&origin,&direction);
            break;
        case ENTT_ENEMY: {
            Entity *enemyEntity = sceneNodeMgr->getEntity((Enemy*)possibleTarget);
            distance = colMgr->rayCollideWithTransform(&origin,&direction,enemyEntity);
            break;
        }
        case ENTT_SHIP: {
            Entity *shipEntity = sceneNodeMgr->getEntity(shipState);
            distance = colMgr->rayCollideWithTransform(&origin,&direction,shipEntity);
        }
    }
    if (distance < 0.0) distance = 1000;
    return distance;
}

double BulletManager::findTarget(IBulletOwner *owner, IBulletTarget **target) {

    IBulletTarget *bestTarget = new MapTarget();
    double tempDistance, shortestDistance;

    // Use the distance to the map as a starting point
    shortestDistance = getDistanceTo(bestTarget,owner);

    std::vector<Enemy*> ents = swarmMgr->getAllLocalEnemies();
    for(std::vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
       Enemy *e = *it;
       tempDistance = getDistanceTo(e,owner);
       if (tempDistance < shortestDistance) {
            shortestDistance = tempDistance;
            bestTarget = e;
        }
    }

    tempDistance = getDistanceTo(shipState,owner);
    if (tempDistance < shortestDistance) {
        shortestDistance = tempDistance;
        bestTarget = shipState;
    }

    tempDistance = getDistanceTo(objective,owner);
    if (tempDistance < shortestDistance) {
        shortestDistance = tempDistance;
        bestTarget = objective;
    }

    *target = bestTarget;
    return shortestDistance;
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets->size();i++) {
        Bullet *b = activeBullets->at(i);
        b->updateLocation();
        if(b->distanceTravelled>b->distanceToTravel) {
            // Bullet has reached it's target
            EffectType hitEffect = b->getTarget()->getHitEffectType();
            Vector3 pointOfImpact = b->getPointOfImpact();
            particleSystemEffectManager->createEffect(hitEffect, pointOfImpact);
            applyDamage(b);
            updateStats(b->getOwner(),b->getTarget());
            delete b;
            activeBullets->erase(activeBullets->begin()+(i));
        }
    }
}

void BulletManager::applyDamage(Bullet *b) {
    IBulletTarget *target = b->getTarget();
    EntityType ownerType = b->getOwner()->getEntityType();
    EntityType targetType = target->getEntityType();
    int damage = b->getDamage();

    if (ownerType == targetType) {
        // Friendly fire!
    } else if (ownerType == ENTT_ENEMY && targetType == ENTT_OBJECTIVE) {
        // Enemy shooting objective
        //target->damage(damage);
    } else {
        target->damage(damage);
    }
}

void BulletManager::handleGun(GunState *gun) {
    if (!gun) return;

    if (gun->fire()) {
        fire(gun);
    }
}

void BulletManager::handleEnemies(std::vector<Enemy*> ents) {
    for(std::vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
        Enemy *e = *it;

        if(e->fire) {
            e->fire = false;
            if(activeBullets->size() < 7) {
                //std::cout << "ENEMY FIRE\n";
                fire(e);
            }
        }
    }
}

void BulletManager::updateStats(IBulletOwner *owner, IBulletTarget *target) {
    if (owner->getEntityType() != ENTT_PLAYER) return;

    PlayerStats *stats = ((GunState*) owner)->stats;
    if (stats == 0) return;

    stats->shotsFired ++;
    if (target->getEntityType() != ENTT_MAP) stats->shotsHit ++;
    if (target->getHealth() <=0) {
        if (target->getEntityType() == ENTT_OBJECTIVE)
            stats->enemiesDestroyed = stats->enemiesDestroyed + 100;
        else
            stats->enemiesDestroyed ++;
    }
}

BulletManager::~BulletManager() {
    cout << "...activeBullets" << endl;
    delete activeBullets;
}
