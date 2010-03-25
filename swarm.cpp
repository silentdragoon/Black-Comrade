
#include "swarm.h"
#include "main.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
	Real roll, Real pitch, Real yaw, ShipState *shipState,
	SceneNodeManager *sceneNodeMgr, Lines *lines, CollisionManager *collisionMgr,
    MapManager *mapMgr, GameParameterMap *gameParameterMap, ParticleSystemEffectManager *particleSystemEffectManager,
    SoundManager *soundMgr)
	: size(size)
	, id(id)
	, location(location)
	, sceneMgr(sceneMgr)
	, speed(ConstManager::getFloat("enemy_patrol_speed") * 
	    ConstManager::getFloat("tick_period"))
	, state(SS_PATROL)
	, shipState(shipState)
    , sceneNodeMgr(sceneNodeMgr)
    , lines(lines)
    , collisionMgr(collisionMgr) 
    , mapMgr(mapMgr)
    , gameParameterMap(gameParameterMap)
    , particleSystemEffectManager(particleSystemEffectManager)
    , soundMgr(soundMgr)
{
    pathFinder = new PathFinder(mapMgr);
    path = std::vector<MapTile*>();
    target = location;
    oldSwarmTile = 0;
	rRayQuery = new RayQuery( sceneMgr );

	// Seed random generator
	rng.seed();

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        std::stringstream out;
        out << id << i;
        ename += out.str();

        Enemy *e = new Enemy(1,i);
        //e->setPosition(Vector3(1400+ 9*i*cos(0),0,250.632+9*i*sin(0)));
        //e->setPosition(location+i*Vector3(0,1,0));
        e->setPosition(location);
        e->roll = roll;
        //e->pitch = pitch;
        e->pitch = pitch + 0.01 * i;
        
        // Add initial random fire delay
        e->fireDelay = genFireDelay();
        
        e->yaw = yaw;
        
        sceneNodeMgr->createNode(e);

        members.push_back(e);
    }
    currentSwarmTile = mapMgr->getMapTile(&location);
}

std::vector<Enemy*> Swarm::getAllEnemies() {
    Enemy *e;
    std::vector<Enemy*> out = std::vector<Enemy*>();
    for(std::deque<Enemy*>::const_iterator it=members.begin();it!=members.end();++it) {
        e = *it;
        out.push_back(e);
    }

    return out;
}

void Swarm::tick()
{
    // Removing must be done before marking (for syncing)
    removeDeadEnemies();
    
    markDeadEnemies();
    
    if(canSwarmSeeShip()) state = SS_ATTACK;
    else {
        MapTile *shipTile = mapMgr->getMapTile(shipState->getPosition());
        Vector3 swarmPosition = getAveragePosition();
        MapTile *swarmTile = mapMgr->getMapTile(&swarmPosition);
        if(!swarmTile) {
            cerr << "Location not within map\n";
        } else {
            if(shipTile != swarmTile) state = SS_PATROL;
        }
    }

    switch(state) {
        case SS_PATROL:
         
            updateTargetLocation();
    
            updateEnemyLocationsFlocking();

            break;
        case SS_ATTACK:

            updateEnemyLocationsAttack();

            shootAtShip();

            break;
    }
}

Swarm::~Swarm()
{
}

Vector3 Swarm::getAverageAlignment()
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    std::deque<Enemy*>::iterator it;
    for(it = members.begin();it != members.end(); it++) {
        Enemy *e = *(it);
        Vector3 direction = e->getDirection();
        x += direction.x;
        y += direction.y;
        z += direction.z;
    }

    return Vector3(x/size,y/size,z/size);
}

Vector3 Swarm::getAveragePosition()
{
    std::deque<Enemy*>::iterator i;
    Enemy *e;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    for(i = members.begin(); i != members.end(); ++i) {
        e = *i;
        x = x + e->getPosition()->x;
        y = y + e->getPosition()->y;
        z = z + e->getPosition()->z;
    }
    return Vector3(x/size,y/size,z/size);
}

bool Swarm::canSwarmSeeShip()
{
    Vector3 orient = 
        SceneNodeManager::directionToOrientationVector(getAverageAlignment());
    
    float yaw = orient.y;

	Vector3 lookDirection(sin(yaw),0,cos(yaw));
	
	// Draw debug lines
	/*Vector3 end1(sin(yaw+ConstManager::getFloat("enemy_sight_angle"))
	    ,0, cos(yaw+ConstManager::getFloat("enemy_sight_angle")));
	
	end1 *= ConstManager::getFloat("enemy_sight_dist");
	
	end1 += getAveragePosition();
	    
	Vector3 end2(sin(yaw-ConstManager::getFloat("enemy_sight_angle"))
	    ,0, cos(yaw-ConstManager::getFloat("enemy_sight_angle")));
	
	end2 *= ConstManager::getFloat("enemy_sight_dist");
	
	end2 += getAveragePosition();
	
	Vector3 avgPos(getAveragePosition());
	
	lines->addLine(&avgPos, &end1);
	lines->addLine(&avgPos, &end2);
	*/
	Radian sightAngle(ConstManager::getFloat("enemy_sight_angle"));
	
	Vector3 lineToShip = *(shipState->getPosition()) - getAveragePosition();
	
	if(lineToShip.length() < ConstManager::getFloat("enemy_sight_dist")) {
		
		Vector3 pos = getAveragePosition();
		Vector3 dir = getAverageAlignment();
		
		double t = collisionMgr->getRCMapDist(&pos, &dir);
		
		if(lineToShip.length() <= t && lineToShip.angleBetween(lookDirection) 
				< sightAngle) {
			return true;
		}
	}
	
	return false;
} 

void Swarm::markDeadEnemies()
{
    for(int i=0;i<members.size();i++) {
    	Enemy *e = members.at(i);
        if(e->health <= 0) {
            //Mark Enemy as Dead
            e->isDead = true;
        } 
    }
}

void Swarm::removeDeadEnemies()
{
    for(int i=0;i<members.size();i++) {
    	Enemy *e = members.at(i);
        if(e->isDead) {
            //Make Explosion here
            Vector3 pos = *e->getPosition();
            particleSystemEffectManager->createExplosion(pos);
            soundMgr->playSound(ConstManager::getInt("sound_explosion"),pos,2.5);
            sceneNodeMgr->deleteNode(e);
        	delete e;
        	members.erase(members.begin()+(i));
            size--;
        	std::cout << "Remove\n";
        }
    }
}

void Swarm::updateTargetLocation() {
    if (members.size() == 0) return;
    MapTile *shipTile = mapMgr->getMapTile(shipState->getPosition());
    Vector3 swarmPosition = getAveragePosition();
    MapTile *swarmTile = mapMgr->getMapTile(&swarmPosition);
    MapTile *targetTile = mapMgr->getMapTile(&target);

    if (currentSwarmTile != swarmTile) {
        oldSwarmTile = currentSwarmTile;
        currentSwarmTile = swarmTile;
    }

    // Find the ship
    if(gameParameterMap->getParameter("SWARMS_FIND_SHIP")) {
        if (oldShipTile != shipTile) {
            // New path must be found
            oldShipTile = shipTile;
            path = pathFinder->findPath(swarmTile,shipTile);
        }
        if (path.size() == 1 || path.size() == 2) {
            // In the current tile or adjacent tile
            target = mapMgr->getActualPosition(shipTile);
            target = *shipState->getPosition();
        } else if (path.size() >=3) {
            // At least 1 tile between the swarm and the ship
            if (targetTile == swarmTile) {
                path.erase(path.begin()+1);
            }
            targetTile = path.at(1);
            target = mapMgr->getActualPosition(targetTile);
        }
    } else { // Move through the map randomly
        if (targetTile == swarmTile) {
            targetTile = pathFinder->pickNextTile(swarmTile,oldSwarmTile);
            target = mapMgr->getActualPosition(targetTile);
        }
    }
}

void Swarm::shootAtShip()
{
	std::deque<Enemy*>::iterator i;
	Enemy *e;	
	
	for(i = members.begin(); i != members.end(); ++i) {
		e = *i;

            e->fire = false;

            Vector3 lineToShip = *(shipState->getPosition()) - *e->getPosition();
            float angleTo = lineToShip.angleBetween(e->getDirection()).valueRadians();

            if(lineToShip.length() < ConstManager::getFloat("enemy_sight_dist") && 
                    angleTo < ConstManager::getFloat("enemy_sight_angle")) {
                if(e->fireDelay <= 0) {
              
                    e->fireDelay = genFireDelay();
                    e->fire = true;
                    
                    e->yawScatter = genScatterAngle();
                    e->pitchScatter = genScatterAngle();
                    
                    //std::cout << "Bang!\n";
                }
            }

            if(e->fireDelay > 0) e->fireDelay -= 1;
    }
}

int Swarm::genFireDelay()
{
    double meanInv = (ConstManager::getFloat("enemy_fire_rate") 
        * ConstManager::getFloat("tick_period"));

    double mean = (meanInv) ? (1 / meanInv) : 100;

    boost::normal_distribution<> nd(mean, 7);

    boost::variate_generator<boost::mt19937&, 
                            boost::normal_distribution<> > var_nom(rng, nd);

    double d = var_nom();
    
    return (int)d;
}

float Swarm::genScatterAngle()
{
    boost::normal_distribution<> nd(0, 
    	ConstManager::getFloat("enemy_inaccuracy"));

    boost::variate_generator<boost::mt19937&, 
                            boost::normal_distribution<> > var_nom(rng, nd);

    double d = var_nom();
    
    return (float)d;
}

void Swarm::turnEnemy(Enemy *e)
{
	Vector3 result;
	Vector3 avg(0,0,0);
	int count = 0;
	Enemy *otherEnemy;
	std::deque<Enemy*>::iterator itr;
	
	float yaw = e->yaw;
	float pitch = e->pitch;
	
	// Add target vector in
	Vector3 t = target - *e->getPosition();
	t.normalise();
	t *= ConstManager::getFloat("flock_target_weight");
	avg += t;
	count++;
	
	t.normalise();
	t *= 8;
	t = *e->getPosition() + t;
	
	/*if(e == members[0]) {
    	lines->addLine(e->getPosition(),&target);
    	lines->addCross(&target);
	}*/
	
	// Add target for forward momentum over all friends in sight range
	
	for(itr = members.begin(); itr != members.end(); ++itr) {
		otherEnemy = *itr;
		if(otherEnemy != e) {
		    Vector3 dist = *otherEnemy->getPosition() - *e->getPosition();
		    // Check that i can see my friend/myself
		    if(dist.length() <= ConstManager::getFloat("flock_detect_dist")) {
		        Vector3 momentum =
		            SceneNodeManager::rollPitchYawToDirection(0.0,
		            otherEnemy->pitch, otherEnemy->yaw);
	            momentum.normalise();
	            momentum *= 
	            ConstManager::getFloat("flock_friend_direction_weight");
	            avg += momentum;
	            count++;
	        }
	    }
	}
		
	Vector3 momentum = SceneNodeManager::rollPitchYawToDirection(0.0,pitch,yaw);
	momentum.normalise();
	momentum *= ConstManager::getFloat("flock_momentum_weight");
	avg += momentum;
	count++;
	
	// Move towards friends
	for(itr = members.begin(); itr != members.end(); ++itr) {
		otherEnemy = *itr;
		if(otherEnemy != e) {
		    Vector3 dist = *otherEnemy->getPosition() - *e->getPosition();
		    // Check that i can see my friend
		    if(dist.length() <= ConstManager::getFloat("flock_detect_dist")) {
		    
		        Vector3 v = dist;
	            v.normalise();
	            float weight;

		        // Should I move closer or further away?
		        if(dist.length() > ConstManager::getFloat("flock_seperation")) {
		            weight = ConstManager::getFloat("flock_friend_closer_weight")
		                * pow((dist.length() - 
		                ConstManager::getFloat("flock_seperation"))/
		                (ConstManager::getFloat("flock_detect_dist") / 
		                ConstManager::getFloat("flock_seperation")),2);
		           
		        } else {
		            weight = - ConstManager::getFloat
		                ("flock_friend_further_weight")
		                * pow(1 - dist.length()/
		                ConstManager::getFloat("flock_seperation"),2);
		        }
		        v *= weight;
                avg += v;
                count++;
		    }
		}
	}
	
	// Send out rays to find obsticals
	float dist;
	// Horizontal ring
	for(int j = -4; j <=4; ++j) {
	    float a = j * PI / 6;
	    Vector3 left(sin(a+yaw),0,cos(a+yaw));
	    left.normalise();
	    Vector3 p = (*e->getPosition()+2*left);
	    dist = collisionMgr->getRCMapDist(&p, &left);
	    //dist = rRayQuery->RaycastFromPoint(p, left, result);
	    result = p + dist * left;
	    if(dist > 0 && dist <= ConstManager::getFloat("flock_seperation")) {
	        Vector3 wall = -(result - *e->getPosition());
	        float weight = ConstManager::getFloat("flock_avoid_wall_weight") * 
	            pow(1 - dist/ConstManager::getFloat("flock_seperation"),2);
	        wall.normalise();
	        wall *= weight;
	        avg = avg + wall;
	        count++;
	        //lines->addLine(e->getPosition(),&result);
	    }
	}
	// Vertical ring
	for(int j = -4; j <=4; ++j) {
	    float a = j * PI / 6;
	    if(j == 0) continue;
	    Vector3 left(0,sin(a+yaw),cos(a+yaw));
	    left.normalise();
	    Vector3 p = (*e->getPosition()+2*left);
	    dist = collisionMgr->getRCMapDist(&p, &left);
	    //dist = rRayQuery->RaycastFromPoint(p, left, result);
	    result = p + dist * left;
	    if(dist > 0 && dist <= ConstManager::getFloat("flock_seperation")) {
	        Vector3 wall = -(result - *e->getPosition());
	        float weight = ConstManager::getFloat("flock_avoid_wall_weight") * 
	            pow(1 - dist/ConstManager::getFloat("flock_seperation"),2);
	        wall.normalise();
	        wall *= weight;
	        avg = avg + wall;
	        count++;
	        //lines->addLine(e->getPosition(),&result);
	    }
	}
	
	// Needs to be done on a per enemy basis (not per swarm)

    float targetYaw;
    float targetPitch;
	if(count) {
	    avg = avg / count;
	    Vector3 orient = SceneNodeManager::directionToOrientationVector(avg);
	    targetYaw = orient.y;
	    targetPitch = orient.x;
		
		yaw = calcNewAngle(yaw, targetYaw,
		    ConstManager::getFloat("flock_turn_rate"));
		
		pitch = calcNewAngle(pitch, targetPitch,
		    ConstManager::getFloat("flock_turn_rate"));
	}
	
	e->yaw = yaw;
	e->pitch = pitch;
}

float Swarm::calcNewAngle(float old, float target, float step)
{
    float result = old;

    while(target < 0) target += 2.0*PI;
		
	while(old < 0) old += 2.0*PI;
    while(old > 2.0*PI) old -= 2.0*PI;
	float posDis = (target >= old) ? target - old : 2*PI + target - old;
	float negDis = (target <= old) ? old - target : 2*PI + old - target;
	
	float move = (posDis <= negDis) ? posDis : -negDis;
	
	if(abs(move) < step) result += move;
	else if(move > 0) result += step;
	else result -= step;
	
	return result;
}

void Swarm::updateEnemyLocationsFlocking()
{
	std::deque<Enemy*>::iterator i;
	Enemy *e;
	
    for(int i=0;i<=(members.size()/3);i++) {
        if(!members.empty()) {		
            e = members.front();
            members.pop_front();
            turnEnemy(e);
            members.push_back(e);
        }
    }
    
	for(i = members.begin(); i != members.end(); ++i) {
		e = *i;
		
		Vector3 newPosition = *e->getPosition();
		Vector3 direction = 
		    SceneNodeManager::rollPitchYawToDirection(0.0,e->pitch,e->yaw);
		direction.normalise();
        
        newPosition += speed * direction;
        
        e->setPosition(newPosition);
    }
	
}

void Swarm::updateEnemyLocationsAttack()
{	
	std::deque<Enemy*>::iterator i;
	Enemy *e;	
	
	for(i = members.begin(); i != members.end(); ++i) {
		e = *i;
		
		attackProcess(e);
		pointAtShip(e);
	}
}

void Swarm::pointAtShip(Enemy *e)
{
    Vector3 lineToShip = *shipState->getPosition() - *e->getPosition();
    
    Vector3 orientation = 
        SceneNodeManager::directionToOrientationVector(lineToShip);
        
    float newYaw = orientation.y;
    float newPitch = orientation.x;
    
    float yaw = e->yaw;
    float pitch = e->pitch;
    
    yaw = calcNewAngle( yaw, newYaw, ConstManager::getFloat("enemy_max_turn"));
    pitch = 
        calcNewAngle( pitch, newPitch, ConstManager::getFloat("enemy_max_turn"));
    e->yaw = yaw;
    e->pitch = pitch;
}

void Swarm::attackProcess(Enemy *e)
{
	Vector3 v;
	Vector3 avg(0,0,0);
	Enemy *otherEnemy;
	std::deque<Enemy*>::iterator itr;
	float weight;
	int count = 0;
	
	float yaw = e->yaw;
	float pitch = e->pitch;
	
	// Move to the ship
	v = *shipState->getPosition() - *e->getPosition();
	
	if(v.length() > ConstManager::getFloat("enemy_ship_target_dist")) {
        weight = 10
        * pow((v.length() - 
        ConstManager::getFloat("enemy_ship_target_dist"))/
        (ConstManager::getFloat("enemy_sight_dist") /
        ConstManager::getFloat("enemy_ship_target_dist")),2);

    } else {
        weight = - 50
        * pow(1 - v.length()/
        ConstManager::getFloat("enemy_ship_target_dist"),2);
    }
    v.normalise();
    v *= weight;
    avg += v;
    ++count;
	
	// Avoid other enemies
	for(itr = members.begin(); itr != members.end(); ++itr) {
		otherEnemy = *itr;
		if(otherEnemy != e) {
		    Vector3 dist = *otherEnemy->getPosition() - *e->getPosition();
		    // Check that i can see my friend
		    if(dist.length() <= ConstManager::getFloat("flock_detect_dist")) {
		    
		        Vector3 v = dist;
	            v.normalise();
	            float weight;

		        // Should I move closer or further away?
		        if(dist.length() > ConstManager::getFloat("enemy_attack_seperation")) {
		            weight = 100
		                * pow((dist.length() - 
		                ConstManager::getFloat("enemy_attack_seperation"))/
		                (ConstManager::getFloat("flock_detect_dist") / 
		                ConstManager::getFloat("enemy_attack_seperation")),2);
		           
		        } else {
		            weight = - 100
		                * pow(1 - dist.length()/
		                ConstManager::getFloat("enemy_attack_seperation"),2);
		        }
		        v *= weight;
                avg += v;
                ++count;
		    }
		}
	}
	
	// Send out rays to find obsticals
	float dist;
	// Horizontal ring
	for(int j = -4; j <=4; ++j) {
	    float a = j * PI / 6;
	    Vector3 left(sin(a+yaw),0,cos(a+yaw));
	    left.normalise();
	    Vector3 p = (*e->getPosition()+2*left);
	    dist = collisionMgr->getRCMapDist(&p, &left);
	    //dist = rRayQuery->RaycastFromPoint(p, left, result);
	    v = p + dist * left;
	    if(dist > 0 && dist <= ConstManager::getFloat("flock_seperation")) {
	        Vector3 wall = -(v - *e->getPosition());
	        float weight = 1000 * 
	            pow(1 - dist/ConstManager::getFloat("flock_seperation"),2);
	        wall.normalise();
	        wall *= weight;
	        avg = avg + wall;
	        count++;
	        //lines->addLine(e->getPosition(),&result);
	    }
	}
	// Vertical ring
	for(int j = -4; j <=4; ++j) {
	    float a = j * PI / 6;
	    if(j == 0) continue;
	    Vector3 left(0,sin(a+yaw),cos(a+yaw));
	    left.normalise();
	    Vector3 p = (*e->getPosition()+2*left);
	    dist = collisionMgr->getRCMapDist(&p, &left);
	    //dist = rRayQuery->RaycastFromPoint(p, left, result);
	    v = p + dist * left;
	    if(dist > 0 && dist <= ConstManager::getFloat("flock_seperation")) {
	        Vector3 wall = -(v - *e->getPosition());
	        float weight = 1000 * 
	            pow(1 - dist/ConstManager::getFloat("flock_seperation"),2);
	        wall.normalise();
	        wall *= weight;
	        avg = avg + wall;
	        count++;
	        //lines->addLine(e->getPosition(),&result);
	    }
	}
	
	avg /= count;
	
	float speedFrac = avg.length() / 10;
	
	speedFrac = speedFrac < 1 ? speedFrac : 1;
	
	avg.normalise();
	avg *= speed * speedFrac;
	
	e->setPosition(*e->getPosition() + avg);
}
