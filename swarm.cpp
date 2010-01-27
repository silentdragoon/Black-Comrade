#include "swarm.h"

Swarm::Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr) :
    size(size),
    id(id),
    location(location),
    sceneMgr(sceneMgr)
{
    // Create the invisible lead node
    leadSN = sceneMgr->getRootSceneNode()->createChildSceneNode();
    leadSN->setPosition(location);

    for(int i=0;i<(size);i++) {
        string ename = "follower";
        stringstream out;
        out << id << i;
        ename += out.str();

        Entity *follow = sceneMgr->createEntity(ename, "smallenemy.mesh");
        SceneNode *followSN = sceneMgr->getRootSceneNode()->createChildSceneNode();
        followSN->attachObject(follow);

        double x = location.x;
        double y = location.y;
        double z = location.z;

        Vector3 offset = Vector3(x,y,z+i+1);

        followSN->setPosition(offset);

        members.push_back(followSN);
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

    vector<SceneNode*>::iterator it;
    for(it = members.begin();it != members.end(); it++) {
        SceneNode *s = *(it);
        Quaternion orient = s->getOrientation();
        Vector3 direction = orient.zAxis();
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

void Swarm::updateLocation()
{
    // TODO: need to add the swarm behaviour here
    // SEPARATION : To avoid flockmates
    // ALIGNMENT : Steer towards the average heading of local flockmates
    // COHESION : Steer to move towards the average position of local flockmates
}
