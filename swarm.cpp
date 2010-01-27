
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

        Enemy *e = new Enemy(followSN,100);

        members.push_back(e);
    }
}

void Swarm::tick()
{
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
	Vector3 lookDirection(cos(yaw),0,sin(yaw));
	
	Radian sightAngle(Const::ENEMY_SIGHT_ANGLE);
	
	Vector3 lineToShip = *(shipState->position) -location;
	
	if(lineToShip.length() < Const::ENEMY_SIGHT_DIS) {
		if(lineToShip.angleBetween(lookDirection) < sightAngle) {
			return true;
		}
	} 
	
	return false;
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

	    yaw +=   1.0f/2.0f*atan(tmp/(speed*Const::LOOKA));

	    location.x += speed * sin(yaw);
	    location.z += speed * cos(yaw);
	}
	
	if(state == SS_ATTACK) {
		
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
