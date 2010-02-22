
#include "swarm.h"
#include "main.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
	Real roll, Real pitch, Real yaw, ShipState *shipState,
	SceneNodeManager *sceneNodeMgr, Lines *lines, CollisionManager *collisionMgr,
    MapManager *mapMgr, GameParameterMap *gameParameterMap)
	: size(size)
	, id(id)
	, location(location)
	, sceneMgr(sceneMgr)
	, roll(roll)
	, pitch(pitch)
	, yaw(yaw)
	, speed(ConstManager::getFloat("enemy_patrol_speed") * 
	    ConstManager::getFloat("tick_period"))
	, state(SS_PATROL)
	, shipState(shipState)
    , sceneNodeMgr(sceneNodeMgr)
    , lines(lines)
    , collisionMgr(collisionMgr) 
    , mapMgr(mapMgr)
    , gameParameterMap(gameParameterMap)
{
    pathFinder = new PathFinder(mapMgr);
    path = std::vector<MapTile*>();
    target = location;
    oldSwarmTile = 0;
	rRayQuery = new RayQuery( sceneMgr );

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        std::stringstream out;
        out << id << i;
        ename += out.str();

        Enemy *e = new Enemy(1,0);
        //e->setPosition(Vector3(1400+ 9*i*cos(0),0,250.632+9*i*sin(0)));
        //e->setPosition(location+i*Vector3(0,1,0));
        e->setPosition(location);
        e->roll = roll;
        //e->pitch = pitch;
        e->pitch = pitch + 0.01 * i;
        
        e->yaw = yaw;
        
        sceneNodeMgr->createNode(e);

        members.push_back(e);
    }
    currentSwarmTile = mapMgr->getMapTile(&location);
}

std::vector<Enemy*> Swarm::getAllEnemies() {
    Enemy *e;
    std::vector<Enemy*> out = std::vector<Enemy*>();
    for(std::vector<Enemy*>::const_iterator it=members.begin();it!=members.end();++it) {
        e = *it;
        out.push_back(e);
    }

    return out;
}

void Swarm::tick()
{
    //std::cout << location << "\t" << yaw << "\n";
    //std::cout << *shipState->getPosition() << "\t" << shipState->yaw << "\n";

    removeDeadEnemies();

    if(isShipInSight()) {
        //state = SS_ATTACK;
    }
	
    // Change speed?
    /*switch(state) {
        case SS_ATTACK:
            speed = ConstManager::getFloat("enemy_attack_speed") *
            ConstManager::getFloat("tick_period");
            break;
        default:
            speed = ConstManager::getFloat("enemy_patrol_speed") *
            ConstManager::getFloat("tick_period");
    }*/

    updateTargetLocation();
    updateSwarmLocation();
    updateEnemyLocations();

    shootAtShip();
}

Swarm::~Swarm()
{
}

Vector3 Swarm::getAverageAlignment()
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    std::vector<Enemy*>::iterator it;
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
    std::vector<Enemy*>::iterator i;
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

bool Swarm::isShipInSight()
{
	Vector3 lookDirection(sin(yaw),0,cos(yaw));
	
	Radian sightAngle(ConstManager::getFloat("enemy_sight_dist"));
	
	Vector3 lineToShip = *(shipState->getPosition()) - getAveragePosition();
	
	if(lineToShip.length() < ConstManager::getFloat("enemy_sight_dist")) {
		if(lineToShip.angleBetween(lookDirection) < sightAngle) {
			return true;
		}
	}
	
	return false;
} 

void Swarm::removeDeadEnemies()
{
   
    for(int i=0;i<members.size();i++) {
    	Enemy *e = members.at(i);
        if(e->health < 0) {
            sceneNodeMgr->deleteNode(e);
        	delete e;
        	members.erase(members.begin()+(i));
            size--;
        	std::cout << "Remove\n";
        } else if (e->health == 0) {
            e->health = -1;
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

void Swarm::updateSwarmLocation()
{
	if(true || state == SS_PATROL) {
	    /*Vector3 result(0,0,0);
	    float dRight, dLeft, tmp;

	    Vector3 futPos( location.x+(speed*ConstManager::getFloat("enemy_look_ahead"))*sin(yaw), location.y, location.z+(speed*ConstManager::getFloat("enemy_look_ahead"))*cos(yaw));

	    Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
	    dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);

	    Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
	    dRight = rRayQuery->RaycastFromPoint(futPos, right, result);

	    tmp = (dLeft + dRight) /2 - dRight;

	    yaw += 1.0f/2.0f*atan(tmp/(speed*
	        ConstManager::getFloat("enemy_look_ahead')));

	    location.x += speed * sin(yaw);
	    location.z += speed * cos(yaw);*/
	}
	else if(state == SS_ATTACK) {
		
		Vector3 lookDirection(sin(yaw),0,cos(yaw));
		
		// Point at ship
		Vector3 lineToShip = *(shipState->getPosition()) -location;
		float newYaw = atan2(lineToShip.x,lineToShip.z);
		if(newYaw < 0) newYaw += 2.0*PI;
		 
		// move yaw to be in the range [0,2PI]
		while(yaw < 0) yaw += 2.0*PI;
	    while(yaw > 2.0*PI) yaw -= 2.0*PI;
		float posDis = (newYaw >= yaw) ? newYaw - yaw : 2*PI + newYaw - yaw;
		float negDis = (newYaw <= yaw) ? yaw - newYaw : 2*PI + yaw - newYaw;
		
		float move = (posDis <= negDis) ? posDis : -negDis;
		
		if(abs(move) < ConstManager::getFloat("enemy_max_turn")) yaw += move;
		else if(move > 0) yaw += ConstManager::getFloat("enemy_max_turn");
		else yaw -= ConstManager::getFloat("enemy_max_turn");
		
		// Check if its pointing at the ship
		float angleTo = lineToShip.angleBetween(lookDirection).valueRadians();
		if(angleTo < PI/2) {
		
			// Move towards ship
			float disToMove = lineToShip.length() - 
			    ConstManager::getFloat("enemy_ship_target_dist");
			float adjustedSpeed = speed * cos(angleTo);
			adjustedSpeed = (abs(disToMove) < adjustedSpeed) ? 
				disToMove : adjustedSpeed;
			
			if(disToMove > 0) { 
				location.x += adjustedSpeed * sin(yaw);
		    	location.z += adjustedSpeed * cos(yaw);
			} else {
				location.x -= adjustedSpeed * sin(yaw);
		    	location.z -= adjustedSpeed * cos(yaw);
			}
		}
		
	}
}

void Swarm::shootAtShip()
{
	std::vector<Enemy*>::iterator i;
	Enemy *e;
	
	i = members.begin(); 
	if(i != members.end()) {
		e = *i;
		
		e->fire = false;
		
		Vector3 lineToShip = *(shipState->getPosition()) - *e->getPosition();
		float angleTo = lineToShip.angleBetween(e->getDirection()).valueRadians();
		
		if(lineToShip.length() < ConstManager::getFloat("enemy_sight_dist") && 
				angleTo < ConstManager::getFloat("enemy_sight_angle")) {
			if(e->fireDelay <= 0) {
				e->fireDelay = 50;
				e->fire = true;
				//std::cout << "Bang!\n";
			}
		}
		
		if(e->fireDelay > 0) e->fireDelay -= 1;
	}
}

void Swarm::turnEnemy(Enemy *e)
{
	Vector3 result;
	Vector3 avg(0,0,0);
	int count = 0;
	Enemy *otherEnemy;
	std::vector<Enemy*>::iterator itr;
	
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
	for(int j = 0; j < 15; ++j) {
	    float a = 2 * j * PI / 15;
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
	for(int j = 0; j < 15; ++j) {
	    float a = 2 * j * PI / 15;
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

void Swarm::updateEnemyLocations()
{

	// Tempory one enemy per swarm solution
	
	std::vector<Enemy*>::iterator i;
	Enemy *e;
	
	
	for(i = members.begin(); i != members.end(); ++i) {
		e = *i;
		
		turnEnemy(e);
		
		Vector3 newPosition = *e->getPosition();
		Vector3 direction = 
		    SceneNodeManager::rollPitchYawToDirection(0.0,e->pitch,e->yaw);
		direction.normalise();
        
        newPosition += speed * direction;
        
        e->setPosition(newPosition);
	}
	
	

	// TODO: need to add the swarm behaviour here
    // SEPARATION : To avoid flockmates
    // ALIGNMENT : Steer towards the average heading of local flockmates
    // COHESION : Steer to move towards the average position of local flockmates
}
