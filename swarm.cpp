
#include "swarm.h"
#include "const.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
	Real roll, Real pitch, Real yaw, ShipState *shipState) 
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
{
	rRayQuery = new RayQuery( sceneMgr );

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        stringstream out;
        out << id << i;
        ename += out.str();

        Entity *follow = sceneMgr->createEntity(ename, "squid.mesh");
        SceneNode *followSN = 
        	sceneMgr->getRootSceneNode()->createChildSceneNode();
        followSN->attachObject(follow);

		followSN->showBoundingBox(true);

        double x = location.x;
        double y = location.y;
        double z = location.z;

        Vector3 offset = Vector3(x,y,z+i+1);

        followSN->setPosition(offset);

        Enemy *e = new Enemy(followSN,1,sceneMgr);

        members.push_back(e);
    }
}

vector<Enemy*> Swarm::getAllEnemies() {
    Enemy *e;
    vector<Enemy*> out = vector<Enemy*>();
    for(vector<Enemy*>::const_iterator it=members.begin();it!=members.end();++it) {
        e = *it;
        out.push_back(e);
    }

    return out;
}

void Swarm::tick()
{
	removeDeadEnemies();

	if(isShipInSight()) {
		state = SS_ATTACK;
	}
	
	// Change speed?
	switch(state) {
		case SS_ATTACK:
			speed = Const::ENEMY_ATTACK_SPEED;
			break;
		default:
			speed = Const::ENEMY_PATROL_SPEED;
	}

	updateSwarmLocation();
	updateEnemyLocations();
}

Swarm::~Swarm()
{
}

Vector3 Swarm::getAverageAlignment()
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    vector<Enemy*>::iterator it;
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
	
	Vector3 lineToShip = *(shipState->position) -location;
	
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
        if(e->health <= 0) {
        	delete e;
        	members.erase(members.begin()+(i));
            size--;
        	std::cout << "Remove\n";
        }
    }
}

void Swarm::updateSwarmLocation()
{
	if(state == SS_PATROL) {
	    Vector3 result(0,0,0);
	    float dRight, dLeft, tmp;

	    Vector3 futPos( location.x+(speed*Const::LOOKA)*sin(yaw), location.y, location.z+(speed*Const::LOOKA)*cos(yaw));

	    Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
	    dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);

	    Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
	    dRight = rRayQuery->RaycastFromPoint(futPos, right, result);

	    tmp = (dLeft + dRight) /2 - dRight;

	    yaw += 1.0f/2.0f*atan(tmp/(speed*Const::LOOKA));

	    location.x += speed * sin(yaw);
	    location.z += speed * cos(yaw);
	}
	
	if(state == SS_ATTACK) {
		
		Vector3 lookDirection(sin(yaw),0,cos(yaw));
		
		// Point at ship
		Vector3 lineToShip = *(shipState->position) -location;
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

void Swarm::updateEnemyLocations()
{

	// Tempory one enemy per swarm solution
	
	vector<Enemy*>::iterator i;
	Enemy *e;
	
	i = members.begin();
	if(i != members.end()) {
		e = *i;
		
		e->setLocation(location);
		e->setOrientation(roll,pitch,yaw);
	}
	

	// TODO: need to add the swarm behaviour here
    // SEPARATION : To avoid flockmates
    // ALIGNMENT : Steer towards the average heading of local flockmates
    // COHESION : Steer to move towards the average position of local flockmates
}
