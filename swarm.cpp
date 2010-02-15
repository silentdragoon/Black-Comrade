
#include "swarm.h"
#include "const.h"
#include "main.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
	Real roll, Real pitch, Real yaw, ShipState *shipState,
	SceneNodeManager *sceneNodeMgr, Lines *lines, CollisionManager *collisionMgr)
	: size(size)
	, id(id)
	, location(location)
	, sceneMgr(sceneMgr)
	, roll(roll)
	, pitch(pitch)
	, yaw(yaw)
	, speed(Const::ENEMY_PATROL_SPEED)
	, state(SS_PATROL)
	, shipState(shipState)
    , sceneNodeMgr(sceneNodeMgr)
    , lines(lines)
    , collisionMgr(collisionMgr)
{

	rRayQuery = new RayQuery( sceneMgr );

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        std::stringstream out;
        out << id << i;
        ename += out.str();

        Enemy *e = new Enemy(1,0);
        e->setPosition(Vector3(1400+ 9*i*cos(0),0,250.632+9*i*sin(0)));
        e->roll = roll;
        e->pitch = pitch;
        e->yaw = 0;
        
        sceneNodeMgr->createNode(e);

        members.push_back(e);
    }
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
	//	state = SS_ATTACK;
	}
	
	// Change speed?
	/*switch(state) {
		case SS_ATTACK:
			speed = Const::ENEMY_ATTACK_SPEED;
			break;
		default:
			speed = Const::ENEMY_PATROL_SPEED;
	}*/

	updateSwarmLocation();
	updateEnemyLocations();
	
	//shootAtShip();
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
    return location;
}

bool Swarm::isShipInSight()
{
	Vector3 lookDirection(sin(yaw),0,cos(yaw));
	
	Radian sightAngle(Const::ENEMY_SIGHT_ANGLE);
	
	Vector3 lineToShip = *(shipState->getPosition()) -location;
	
	if(lineToShip.length() < Const::ENEMY_SIGHT_DIS) {
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

void Swarm::updateSwarmLocation()
{
	if(true || state == SS_PATROL) {
	    /*Vector3 result(0,0,0);
	    float dRight, dLeft, tmp;

	    Vector3 futPos( location.x+(speed*Const::LOOKA)*sin(yaw), location.y, location.z+(speed*Const::LOOKA)*cos(yaw));

	    Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
	    dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);

	    Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
	    dRight = rRayQuery->RaycastFromPoint(futPos, right, result);

	    tmp = (dLeft + dRight) /2 - dRight;

	    yaw += 1.0f/2.0f*atan(tmp/(speed*Const::LOOKA));

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
		
		if(abs(move) < Const::TURN_TO_LOOK_STEP) yaw += move;
		else if(move > 0) yaw += Const::TURN_TO_LOOK_STEP;
		else yaw -= Const::TURN_TO_LOOK_STEP;
		
		// Check if its pointing at the ship
		float angleTo = lineToShip.angleBetween(lookDirection).valueRadians();
		if(angleTo < PI/2) {
		
			// Move towards ship
			float disToMove = lineToShip.length() - Const::SWARM_TARGET_DIS;
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
		
		if(lineToShip.length() < Const::ENEMY_SIGHT_DIS && 
				angleTo < Const::ENEMY_SIGHT_ANGLE) {
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
   #define SIGHT_RADIUS 15
   #define SEPERATION 8
   #define TURN_RATE PI/4
	Vector3 result;
	Vector3 avg(0,0,0);
	int count = 0;
	Enemy *otherEnemy;
	std::vector<Enemy*>::iterator itr;
	
	float yaw = e->yaw;
	
	// Add target for forward momentum over all friends in sight range
	
	for(itr = members.begin(); itr != members.end(); ++itr) {
		otherEnemy = *itr;
		if(otherEnemy != e) {
		    Vector3 dist = *otherEnemy->getPosition() - *e->getPosition();
		    // Check that i can see my friend/myself
		    if(dist.length() <= SIGHT_RADIUS) {
		        Vector3 momentum(sin(otherEnemy->yaw),0,cos(otherEnemy->yaw));
	            momentum.normalise();
	            momentum *= 100;
	            avg += momentum;
	            count++;
	        }
	    }
	}
		
	Vector3 momentum(sin(yaw),0,cos(yaw));
	momentum.normalise();
	momentum *= 1000;
	avg += momentum;
	count++;
	
	// Move towards friends
	for(itr = members.begin(); itr != members.end(); ++itr) {
		otherEnemy = *itr;
		if(otherEnemy != e) {
		    Vector3 dist = *otherEnemy->getPosition() - *e->getPosition();
		    // Check that i can see my friend
		    if(dist.length() <= SIGHT_RADIUS) {
		    
		        Vector3 v = dist;
	            v.normalise();
	            float weight;

		        // Should I move closer or further away?
		        if(dist.length() > SEPERATION) {
		            weight = 100 * pow((dist.length() - SEPERATION)/
		                (SIGHT_RADIUS / SEPERATION),2);
		           
		        } else {
		            weight = - 1000 * pow(1 - dist.length()/SEPERATION,2);
		        }
		        v *= weight;
                avg += v;
                count++;
		    }
		}
	}
	
	// Send out rays to find obsticals
	float dist;
	for(int j = 0; j < 15; ++j) {
	    float a = 2 * j * PI / 15;
	    Vector3 left(sin(a+yaw),0,cos(a+yaw));
	    left.normalise();
	    Vector3 p = (*e->getPosition()+2*left);
	    dist = collisionMgr->getRCMapDist(&p, &left);
	    //dist = rRayQuery->RaycastFromPoint(p, left, result);
	    result = p + dist * left;
	    if(dist > 0 && dist <= SEPERATION) {
	        Vector3 wall = -(result - *e->getPosition());
	        float weight = 1000 * pow(1 - dist/SEPERATION,2);
	        wall.normalise();
	        wall *= weight;
	        avg = avg + wall;
	        count++;
	        //lines->addLine(e->getPosition(),&result);
	    }
	}
	
	// Needs to be done on a per enemy basis (not per swarm)
	
	float newYaw = yaw;
	
	if(count) {
	    avg = avg / count;
	    newYaw = atan2(avg.x,avg.z);
	    //cout << avg.z << "\t" << avg.x << "\n";
	    //cout << yaw << "\t" << newYaw << "\n";
	    if(newYaw < 0) newYaw += 2.0*PI;
		
		// move yaw to be in the range [0,2PI]
		while(yaw < 0) yaw += 2.0*PI;
	    while(yaw > 2.0*PI) yaw -= 2.0*PI;
		float posDis = (newYaw >= yaw) ? newYaw - yaw : 2*PI + newYaw - yaw;
		float negDis = (newYaw <= yaw) ? yaw - newYaw : 2*PI + yaw - newYaw;
		
		float move = (posDis <= negDis) ? posDis : -negDis;
		
		if(abs(move) < TURN_RATE) yaw += move;
		else if(move > 0) yaw += TURN_RATE;
		else yaw -= TURN_RATE;
		
	}
	
	e->yaw = yaw;
	
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
		
		newPosition.x += speed * sin(e->yaw);
        newPosition.z += speed * cos(e->yaw);
        
        e->setPosition(newPosition);
	}
	
	

	// TODO: need to add the swarm behaviour here
    // SEPARATION : To avoid flockmates
    // ALIGNMENT : Steer towards the average heading of local flockmates
    // COHESION : Steer to move towards the average position of local flockmates
}
