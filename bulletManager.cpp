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
    , bnum(0)
{
    activeBullets = new std::vector<Bullet*>();
    colMgr->addMesh( sceneNodeMgr->getEntity(shipState) );
}

void BulletManager::fire(IBulletOwner *owner) {
    SceneNode *bulletNode = makeBulletNode(owner->getBulletColour(),owner->getBulletOrigin());
    
    IBulletTarget *target;
    double dtt = findTarget(owner,&target);
    Bullet *b = new Bullet(owner, target, bulletNode, dtt);
    activeBullets->push_back(b);
    //particleSystemEffectManager->createEffect(owner->getMuzzleFlashEffectType(), Vector3 pos);
}

double BulletManager::getDistanceTo(IBulletTarget *possibleTarget,IBulletOwner *owner) {
    double distance = 0.0;
    Vector3 origin = owner->getBulletOrigin();
    Vector3 direction = owner->getBulletDirection();

    switch (possibleTarget->getEntityType()) {
        case ENTT_OBJECTIVE:
            distance = colMgr->getRCObjDist(&origin,&direction);
            std::cout << "Getting objective distance" << std::endl;
            break;
        case ENTT_MAP:
            distance = colMgr->getRCMapDist(&origin,&direction);
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
    double tempDistance = 0;
    IBulletTarget *bestTarget = new MapTarget();
    double shortestDistance = getDistanceTo(bestTarget,owner);  // Distance to map

    std::vector<Enemy*> ents = swarmMgr->getAllEnemies();
    Enemy *e;
    for(std::vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
       e = *it;
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

bool BulletManager::fire(Vector3 origin, Vector3 direction, ColourValue c, Vector3 trailOrigin) {
    return fire(origin,direction,c, trailOrigin, 0);
}

SceneNode *BulletManager::makeBulletNode(ColourValue bulletColour, Vector3 position) {
    string bullName = "Bullet";
    string bname = "Bill";
    string lname = "Light";
    string rname = "Ribbon";
    std::stringstream out;
    out << bnum++;
    bname += out.str();
    lname += out.str();
    bullName += out.str();
    rname += out.str();

    SceneNode *bulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode(bullName);
    bulletNode->setPosition(position);

    BillboardSet *bbbs = sceneMgr->createBillboardSet(bname,1);
    bbbs->setMaterialName("PE/Streak");
    Billboard *bbb = bbbs->createBillboard(0,0,0,bulletColour);
    bbb->setDimensions(0.7,0.7);

    RibbonTrail *trail = sceneMgr->createRibbonTrail(rname);
    trail->setMaterialName("PE/LightRibbonTrail");
    trail->setTrailLength(75);
    trail->setMaxChainElements(400);
    trail->setInitialColour(0,bulletColour);
    trail->setInitialWidth(0,0.7);

    trail->addNode(bulletNode);

    sceneMgr->getRootSceneNode()->attachObject(trail);

    Light *l = sceneMgr->createLight(lname);
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(bulletColour);
    l->setSpecularColour(bulletColour);
    l->setAttenuation(100,0.5,0.0005,0);

    bulletNode->attachObject(bbbs);
    bulletNode->attachObject(l);

    return bulletNode;
}

bool BulletManager::fire(Vector3 origin, Vector3 direction, ColourValue c, Vector3 trailOrigin, PlayerStats *stats) 
{
    string bullName = "Bullet";
    string bname = "Bill";
    string lname = "Light";
    string rname = "Ribbon";
    std::stringstream out;
    out << bnum++;
    bname += out.str();
    lname += out.str();
    bullName += out.str();
    rname += out.str();

    SceneNode *bulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode(bullName);
    bulletNode->setPosition(trailOrigin);

    BillboardSet *bbbs = sceneMgr->createBillboardSet(bname,1);
    bbbs->setMaterialName("PE/Streak");
    Billboard *bbb = bbbs->createBillboard(0,0,0,c);
    bbb->setDimensions(0.7,0.7);

    RibbonTrail *trail = sceneMgr->createRibbonTrail(rname);
    trail->setMaterialName("PE/LightRibbonTrail");
    trail->setTrailLength(75);
    trail->setMaxChainElements(400);
    trail->setInitialColour(0,c);
    trail->setInitialWidth(0,0.7);
    trail->addNode(bulletNode);
    //sceneMgr->getRootSceneNode()->attachObject(trail);

    Light *l = sceneMgr->createLight(lname);
    l->setType(Light::LT_POINT);
    l->setDiffuseColour(c);
    l->setSpecularColour(c);
    l->setAttenuation(100,0.5,0.0005,0);

    //bulletNode->attachObject(bbbs);
    bulletNode->attachObject(l);

    Vector3 *pos = new Vector3(trailOrigin.x,trailOrigin.y,trailOrigin.z);
    particleSystemEffectManager->createMuzzleFlash(*pos);

    double t = colMgr->getRCMapDist(pos,&direction);
    if(t<0) t=10000;

    bool isEnemy = false;
    Enemy *hurtEnemy = NULL;
    if(swarmMgr) {
        std::vector<Enemy*> ents = swarmMgr->getAllEnemies();
        Enemy *e;
        for(std::vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
            e = *it;
            double temp = colMgr->rayCollideWithTransform(&origin,&direction,sceneNodeMgr->getEntity(e));
            if(temp<t && temp > 0.0) {
                t = temp;
                isEnemy = true;
                hurtEnemy = e;
            }
        }
    }

    bool isShip = false;
    Entity *shipEntity = sceneNodeMgr->getEntity(shipState);
    double distToShip = colMgr->rayCollideWithTransform(pos,&direction,shipEntity);
    if (distToShip < t && distToShip > 1.0) {
        isShip = true;
        isEnemy = false;
    }

    bool isObjective = false;
    double distToObj = colMgr->getRCObjDist(pos,&direction);
    if(distToObj < t && distToObj > 0.0) {
        isObjective = true;
        isEnemy = false;
        isShip = false;
        std::cout << "BC" << std::endl;
        objective->damageObjective();
    }
    
    // FIRE THE BULLET!
    Bullet *b = new Bullet(bulletNode,sceneMgr,bullName,rname,direction,
    	Const::FRONT_BULLET_SPEED,t,stats);

    if (isEnemy) { // Switch enemyFire to not to stop enemy friendly fire
        b->hitEnemy = true;
        b->enemy = hurtEnemy;
    } else if (isShip) b->hitShip = true;

    activeBullets->push_back(b);

    // TODO: Return enum rather than bool so we can record friendly fire
    if (isEnemy)
        return true;
    else
        return false;
}

void BulletManager::updateBullets2() {
    for(int i=0;i<activeBullets->size();i++) {
        Bullet *b = activeBullets->at(i);
        b->updateLocation();
        if(b->distanceTravelled>b->distanceToTravel) {
            // Bullet has reached it's target
            std::cout << "Delete\n";
            applyDamage(b);
            ////particleSystemEffectManager->createEffect(b->getTarget()->getHitEffectType(), Vector3 pos);
            sceneMgr->destroySceneNode(b->getNode());	
            delete b;
            activeBullets->erase(activeBullets->begin()+(i));
        }
    }
}

void BulletManager::applyDamage(Bullet *b) {
    IBulletTarget *target = b->getTarget();
    EntityType ownerType = b->getOwner()->getEntityType();
    EntityType targetType = target->getEntityType();
    int damage = b->getOwner()->getBulletDamage();

    if (ownerType == targetType) {
        // Friendly fire!
        //target->setHealth(target->getHealth()-damage);
    } else {
        target->setHealth(target->getHealth()-damage);
    }
    std::cout << targetType << std::endl;
}

void BulletManager::updateBullets() {
    for(int i=0;i<activeBullets->size();i++) {
        Bullet *b = activeBullets->at(i);
        b->updateLocation();
        if(b->distanceTravelled>b->distanceToTravel) {
            // Hurt Enemy or Ship
            if(b->enemy) {
                b->enemy->health -= 1;
                if (b->playerStats != 0 && b->enemy->health <= 0) {
                    std::cout << "enemy destroyed" << std::endl;
                    b->playerStats->enemiesDestroyed ++;
                }
            } else if (b->hitShip) {
                if (b->playerStats != 0) b->playerStats->friendlyFire ++;
                damageState->damage();
            }
            Vector3 pos = b->getDeathSpark();         
            particleSystemEffectManager->createSparks(pos);
            delete b;
            activeBullets->erase(activeBullets->begin()+(i));
        }
    }
}

BulletManager::~BulletManager() {
    cout << "...activeBullets" << endl;
    delete activeBullets;
}

void BulletManager::handleGun(GunState *gun) {
    if (!gun) return;

    if (gun->fire()) {
        if (gun->stats != 0) gun->stats->shotsFired += 1;

        //if (fire(position,direction,ColourValue(0.7f,0.4f,0.0f), trailPos, gun->stats))
        //if (gun->stats != 0) gun->stats->shotsHit += 1;
        fire(gun);

        playerFire = true;
    }
}

void BulletManager::handleEnemies(std::vector<Enemy*> ents) {
	Enemy *e;
	for(std::vector<Enemy*>::const_iterator it=ents.begin();it!=ents.end();++it) {
	    e = *it;
	    
	    Vector3 angles = SceneNodeManager::directionToOrientationVector(
	    	e->getDirection());
	    
	    float bulletYaw = angles.y + e->yawScatter;
	    float bulletPitch = angles.x + e->pitchScatter;
	    
	    Vector3 bulletDirection = SceneNodeManager::rollPitchYawToDirection(
	    	0, bulletPitch, bulletYaw);
	        
	    if(e->fire) {
            e->fire = false;
            if(activeBullets->size()<7) {
                //fire(*e->getPosition(),bulletDirection,ColourValue(0.7f,0.0f,0.0f),*e->getPosition());
                fire(e);
            }
	    }
	}
}

std::vector<Bullet*> * BulletManager::getActiveBullets() {
    return activeBullets;
}

void BulletManager::tick()
{
    updateBullets2();

    playerFire = false;
    enemyFire = false;

    // Guns shoot if neccessary
    handleGun(pilotGunState);
    //handleGun(engineerGunState);
    //handleGun(navigatorGunState);
    
    // Enemies shoot if neccessary
    //handleEnemies(swarmMgr->getAllEnemies());
    //handleEnemies(swarmMgr->getReplicatedEnemies());  
    
}
