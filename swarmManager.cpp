#include "swarmManager.h"

SwarmManager::SwarmManager(SceneManager *sceneMgr, GameParameterMap *gamePM) :
    sceneMgr(sceneMgr),
    gamePM(gamePM),
    id(0)
{
    // TODO: CREATE INTIAL PATROLLING DUDES
}

SwarmManager::~SwarmManager()
{

}

void SwarmManager::createSwarm(int size, Vector3 location)
{
    Swarm *s = new Swarm(size,id,location,sceneMgr);
    activeSwarms->push_back(s);
    id++;
}

void SwarmManager::tick() 
{
    string *sp = gamePM->getParameter("SPAWN");

}
