#include "swarmManager.h"

SwarmManager::SwarmManager(SceneManager *sceneMgr) :
    sceneMgr(sceneMgr),
    id(0)
{

}

SwarmManager::~SwarmManager()
{

}

void SwarmManager::createSwarm(int size, Vector3 location)
{
    Swarm *s = new Swarm(size,id,location,sceneMgr,0,0,0);
    id++;
}

void SwarmManager::tick() 
{

}
