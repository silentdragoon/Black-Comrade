#include "swarmManager.h"
#include "const.h"

SwarmManager::SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr,
	GameParameterMap *gamePM, MapManager *mapMgr, ShipState *shipState,
	CollisionManager* colMgr, NetworkingManager *networkingMgr, Lines *lines,
    GameStateMachine *gameStateMachine, ParticleSystemEffectManager *particleSystemEffectManager,
    SoundManager *soundMgr) :
    sceneMgr(sceneMgr),
    sceneNodeMgr(sceneNodeMgr),
    gamePM(gamePM),
    mapMgr(mapMgr),
    id(0),
    dynSwarmSize(0),
    swarmTick(0),
    shipState(shipState),
    colMgr(colMgr),
    networkingMgr(networkingMgr),
    gameStateMachine(gameStateMachine),
    particleSystemEffectManager(particleSystemEffectManager),
    soundMgr(soundMgr),
    lines(lines)
{

    activeSwarms = std::deque<Swarm*>();

    std::vector<Vector3*> wps = mapMgr->getInitialSpawnPoints();

    Vector3 *v;
    for(std::vector<Vector3*>::const_iterator it=wps.begin();it!=wps.end(); ++it) {
        v = *it;
        std::vector<Vector3*> rings = mapMgr->getSpawnPoints(v);

        Vector3 *spawnPoint;
        for(std::vector<Vector3*>::const_iterator ite=rings.begin();ite!=rings.end(); ++ite) {
            spawnPoint = *ite;
            Vector3 sp = Vector3(spawnPoint->x,spawnPoint->y,spawnPoint->z);
            createSwarm(ConstManager::getInt("swarm_size"),sp);
            cout << "Created initial swarm..." << endl;
            return;
        }
    }
    
}

SwarmManager::SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
    NetworkingManager *networkingMgr, ParticleSystemEffectManager *particleSystemEffectManager,
    SoundManager *soundMgr) :
    sceneMgr(sceneMgr),
    sceneNodeMgr(sceneNodeMgr),
    gamePM(gamePM),
    mapMgr(0),
    id(0),
    dynSwarmSize(0),
    swarmTick(0),
    shipState(shipState),
    networkingMgr(networkingMgr),
    particleSystemEffectManager(particleSystemEffectManager),
    soundMgr(soundMgr),
    lines(0)
{
    activeSwarms = std::deque<Swarm*>();
}

SwarmManager::~SwarmManager()
{
}

void SwarmManager::createSwarm(int size, Vector3 location)
{
    std::cout << "Created Swarm of size: " << size << std::endl;
    Swarm *s = new Swarm(size,id,location,sceneMgr,0,0,0,shipState,sceneNodeMgr
        ,lines,colMgr,mapMgr,gamePM,particleSystemEffectManager,soundMgr);

    std::vector<Enemy*> ents = s->getAllEnemies();
    Enemy *en;
    for(std::vector<Enemy*>::const_iterator ite=ents.begin();ite!=ents.end();++ite) {
        en = *ite;
        colMgr->addMesh(sceneNodeMgr->getEntity(en));
    }
    
    activeSwarms.push_back(s);
    id++;
}

std::deque<Swarm*> SwarmManager::getAllSwarms()
{
    return activeSwarms;
}

std::vector<Enemy*> SwarmManager::getAllEnemies()
{
    Swarm *s;
    Enemy *enemy;
    std::vector<Enemy*> out = std::vector<Enemy*>();

    //std::cout << "ACTIVESWARMS SIZE: " << activeSwarms.size() << std::endl;

    for(int i=0; i<activeSwarms.size(); i++) {
        s = activeSwarms.at(i);
        //std::cout << "SWARM SIZE: " << s->size << std::endl;
        std::vector<Enemy*> ents = s->getAllEnemies();

        for(std::vector<Enemy*>::const_iterator ite=ents.begin();ite!=ents.end();++ite) {
            enemy = *ite;
            out.push_back(enemy);
        }
    }

    return out;
}

std::vector<Enemy*> SwarmManager::getReplicatedEnemies() {
    Enemy *enemy;
    std::vector<Enemy*> out = std::vector<Enemy*>();

    std::vector<ReplicaObject*> replicatedEnemies = networkingMgr->getReplicas("Enemy");
    for (std::vector<ReplicaObject*>::const_iterator it=replicatedEnemies.begin();it!=replicatedEnemies.end();++it) {
        enemy = (Enemy*) *it;
        out.push_back(enemy);
    }

    return out;
}

void SwarmManager::updateRemoteSwarms() {
    if (mapMgr == 0) {
        std::vector<ReplicaObject*> replicatedEnemies = networkingMgr->getReplicas("Enemy");
        for (std::vector<ReplicaObject*>::const_iterator it=replicatedEnemies.begin();it!=replicatedEnemies.end();++it) {
            Enemy *enemy = (Enemy*) *it;

            if (enemy->isDead) {
                //Make Explosion here
                Vector3 *pos = enemy->getPosition();
                particleSystemEffectManager->createExplosion(*pos);
                sceneNodeMgr->deleteNode(enemy);
                delete enemy;
            } else {
                sceneNodeMgr->createNode(enemy);
            }
        }
    } else {
        std::vector<Enemy*> allEnemies = getAllEnemies();
        for (std::vector<Enemy*>::const_iterator it = allEnemies.begin(); it!=allEnemies.end();++it) {
            if (!(*it)->isReplicated) {
                networkingMgr->replicate(*it);
                (*it)->isReplicated = true;
            }
        }
    }
}

void SwarmManager::tick() 
{
    if(lines) lines->clear();

    updateRemoteSwarms();
    if (mapMgr == 0) return;

    int sp = gamePM->getParameter("SPAWN");


    if(sp>0) {
        if(activeSwarms.size()<sp) {
            swarmTick++;
            if(swarmTick>Const::SPAWN_DELAY) {
                swarmTick = 0;
                Vector3 spawnPoint = mapMgr->getDynamicSpawnPoint(shipState->getPosition());
                createSwarm(ConstManager::getInt("swarm_size"),spawnPoint);
                cout << "Swarms: " << activeSwarms.size() << endl;
            }
        }
    }

    // Here we are updating the locations of the swarms and the enemies within
    for(int i = 0; i<=activeSwarms.size(); i++) {
        if(!activeSwarms.empty()) {
            Swarm *s = activeSwarms.front();
            activeSwarms.pop_front();
            if(s->size!=0) {
                s->tick();
                activeSwarms.push_back(s);
                if(s->canSwarmSeeShip()) 
                	gameStateMachine->setIsShipInSight(true);
            }
        }
    }
}

