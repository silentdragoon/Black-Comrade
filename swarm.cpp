
#include "swarm.h"
#include "const.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
	Real roll, Real pitch, Real yaw) 
	: size(size)
	, id(id)
	, location(location)
	, sceneMgr(sceneMgr)
	, roll(roll)
	, pitch(pitch)
	, yaw(yaw)
{
	rRayQuery = new RayQuery( sceneMgr );

    // Create the invisible lead node
    leadSN = sceneMgr->getRootSceneNode()->createChildSceneNode();
    leadSN->setPosition(location);

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        stringstream out;
        out << id << i;
        ename += out.str();

        Entity *follow = sceneMgr->createEntity(ename, "smallenemy.mesh");
        SceneNode *followSN = 
        	sceneMgr->getRootSceneNode()->createChildSceneNode();
        followSN->attachObject(follow);

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
    return leadSN->getPosition();
}

void Swarm::updateSwarmLocation()
{
    Vector3 result(0,0,0);
    float dRight, dLeft, tmp;

    Vector3 futPos( location.x+(Const::FVELOCITY*Const::LOOKA)*sin(yaw), location.y, location.z+(Const::FVELOCITY*Const::LOOKA)*cos(yaw));

    Vector3 left(sin(yaw+1.57),0,cos(yaw+1.57));
    dLeft = rRayQuery->RaycastFromPoint(futPos, left, result);

    Vector3 right(sin(yaw-1.57),0,cos(yaw-1.57));
    dRight = rRayQuery->RaycastFromPoint(futPos, right, result);

    tmp = (dLeft + dRight) /2 - dRight;

    yaw +=   1.0f/2.0f*atan(tmp/(Const::FVELOCITY*Const::LOOKA));

    location.x += Const::FVELOCITY * sin(yaw);
    location.z += Const::FVELOCITY * cos(yaw);
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
