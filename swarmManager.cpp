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
    lines(lines),
    ticksSinceLastUpdate(0)
{

    loadPatrolGroups();

    activeSwarms = std::deque<Swarm*>();
    replicatedEnemies = std::vector<ReplicaObject*>();

    std::vector<Waypoint*> wps = mapMgr->getAllWaypoints();
    
    for(std::vector<Waypoint*>::iterator it = wps.begin();
        it != wps.end(); ++it) {
        Waypoint *wp = *it;
        
        Vector3 pos((wp->getX() + 0.5) * ConstManager::getInt("map_tile_size"), 
            0, (wp->getY() + 0.5) * ConstManager::getInt("map_tile_size"));

        MapTile *mapTile = mapMgr->getMapTile(&pos);

        Vector3 *spawnPoint = mapTile->getSpawnPoints()[0];

        if(wp->getName()->substr(0,8).compare("en_spawn") == 0) {
            
            string tag;
            
            if(wp->getName()->size() > 8) 
                tag = wp->getName()->substr(9);
            
            std::map<string, std::vector<MapTile*> >::iterator it;
             
            it = enemyPatrolBlocks.find(tag);
            
            std::vector<MapTile*> patrolBlocks;
            
            if(it != enemyPatrolBlocks.end()) {
                patrolBlocks = it->second;
            }

            cout << "Created initial swarm @ " << *spawnPoint << endl;
            cout << "Patrol Group: " << tag << endl;
            createSwarm(ConstManager::getInt("swarm_size"), *spawnPoint,
                patrolBlocks);
            
        }
    }    
}

void SwarmManager::loadPatrolGroups()
{
    std::vector<Waypoint*> wps = mapMgr->getAllWaypoints();
    
    for(std::vector<Waypoint*>::iterator it = wps.begin();
        it != wps.end(); ++it) {
        Waypoint *wp = *it;
        
        Vector3 pos((wp->getX() + 0.5) * ConstManager::getInt("map_tile_size"), 
            0, (wp->getY() + 0.5) * ConstManager::getInt("map_tile_size"));
        
        MapTile *mapTile = mapMgr->getMapTile(&pos);

        if(wp->getName()->substr(0,13).compare("patrol_block_") == 0) {
            string tag = wp->getName()->substr(13);
            
            if(enemyPatrolBlocks.count(tag) > 0) {
                enemyPatrolBlocks[tag].push_back(mapTile);
            } else {
                enemyPatrolBlocks.insert(
                    pair<string,std::vector<MapTile*> >(tag, 
                        std::vector<MapTile*>()));
                enemyPatrolBlocks[tag].push_back(mapTile);
                
            }
        }
    }
}

SwarmManager::SwarmManager(SceneManager *sceneMgr, SceneNodeManager *sceneNodeMgr, GameParameterMap *gamePM,
    NetworkingManager *networkingMgr, ParticleSystemEffectManager *particleSystemEffectManager,
    SoundManager *soundMgr, CollisionManager *colMgr) :
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
    lines(0),
    ticksSinceLastUpdate(0),
    colMgr(colMgr)
{
    activeSwarms = std::deque<Swarm*>();
}

SwarmManager::~SwarmManager()
{
}

void SwarmManager::createSwarm(int size, Vector3 location, std::vector<MapTile*> patrolBlocks)
{
    std::cout << "Created Swarm of size: " << size << std::endl;
    Swarm *s = new Swarm(size,id,location,sceneMgr,0,0,0,shipState,sceneNodeMgr
        ,lines,colMgr,mapMgr,gamePM,particleSystemEffectManager,soundMgr
        ,networkingMgr,patrolBlocks);

    std::vector<Enemy*> ents = s->getAllEnemies();
    Enemy *en;
    for(std::vector<Enemy*>::const_iterator ite=ents.begin();ite!=ents.end();++ite) {
        en = *ite;
        colMgr->addEnemyMesh(sceneNodeMgr->getEntity(en));
    }
    
    activeSwarms.push_back(s);
    id++;
}

std::deque<Swarm*> SwarmManager::getAllSwarms()
{
    return activeSwarms;
}

std::vector<Enemy*> SwarmManager::getAllLocalEnemies()
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

    //std::vector<Enemy*> replicas = getReplicatedEnemies();
    
    //for(std::vector<Enemy*>::const_iterator it= replicas.begin(); it != replicas.end(); ++it) {
    //    out.push_back(*it);
    //}

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

std::vector<Enemy*> SwarmManager::getEnemies() {
    if (mapMgr == 0)
        return getReplicatedEnemies();
    else
        return getAllLocalEnemies();
}

void SwarmManager::updateRemoteSwarms() {
    if (mapMgr == 0) {
        
        replicatedEnemies = networkingMgr->getReplicas("Enemy");
        
        for (std::vector<ReplicaObject*>::const_iterator it=replicatedEnemies.begin();it!=replicatedEnemies.end();++it) {
            Enemy *enemy = (Enemy*) *it;

            if (enemy->isDead) {
                if (!enemy->hasExploded) {
                    //Make Explosion here
                    Vector3 *pos = enemy->getPosition();
                    particleSystemEffectManager->createExplosion(*pos);
                    sceneNodeMgr->deleteNode(enemy);
                    enemy->hasExploded = true;
                }
            } else if (!enemy->isReplicated) {
                enemy->isReplicated = true;
                sceneNodeMgr->createNode(enemy);
                colMgr->addEnemyMesh(sceneNodeMgr->getEntity(enemy));
            }
        }
    } else {
        if (ticksSinceLastUpdate <= 100) {
            ticksSinceLastUpdate ++;
            return;
        } else {ticksSinceLastUpdate = 0;}
        
        std::vector<Enemy*> allEnemies = getAllLocalEnemies();
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

    
    // This is broken atm, and it should be done through gameParameterMap anyway!
    //if (gameStateMachine->currentGameState() == GS_TUTORIAL) 
    //{  return; }

    int sp = gamePM->getParameter("SPAWN");


    if(sp>0) {
        if(activeSwarms.size()<sp) {
            swarmTick++;
            if(swarmTick>Const::SPAWN_DELAY) {
                swarmTick = 0;
                Vector3 spawnPoint = mapMgr->getDynamicSpawnPoint(shipState->getPosition());
                createSwarm(ConstManager::getInt("swarm_size"),spawnPoint,
                    std::vector<MapTile*>());
                cout << "Swarms: " << activeSwarms.size() << endl;
            }
        }
    }

    // Here we are updating the locations of the swarms and the enemies within
    for(int i = 0; i<=activeSwarms.size(); i++) {
        if(!activeSwarms.empty()) {
            Swarm *s = activeSwarms.front();
            activeSwarms.pop_front();
            
            Vector3 swarmLocn =  s->getAveragePosition();
            Vector3 *shipLocn = shipState->getPosition();
            float dist = shipLocn->distance(swarmLocn);
            if(dist>ConstManager::getFloat("swarm_respawn_dist")) {
                if((gameStateMachine->currentGameState()==GS_ATTACK)||
                        (gameStateMachine->currentGameState()==GS_BLACK_COMRADE)||
                        (gameStateMachine->currentGameState()==GS_FLEE)) {
                    cout << "Removing left behind swarm" << endl;
                    s->killAllMembers();     
                }
            } 
            if(s->size!=0) {
                s->tick();
                activeSwarms.push_back(s);
                if(s->canSwarmSeeShip()) 
                	gameStateMachine->setIsShipInSight(true);
            }
        }
    }
}

