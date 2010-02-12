#include "sceneNodeManager.h"

SceneNodeManager::SceneNodeManager(SceneManager *sceneMgr)
    : sceneMgr(sceneMgr)
{
    madeShip = false;

}

void SceneNodeManager::tick() {
    std::map<IDrawable*,SceneNode*>::const_iterator itr;
    for (itr = nodes.begin() ; itr != nodes.end() ; itr++) {
        IDrawable *object = itr->first;
        SceneNode *node = itr->second;
        updateNode(object,node);
    }
}

void SceneNodeManager::updateNode(IDrawable *object, SceneNode *node) {
	node->resetOrientation();
    node->setPosition(*object->getPosition());

    Radian roll(object->getOrientation()->z);
    Radian pitch(object->getOrientation()->x);
    Radian yaw(object->getOrientation()->y);

    node->yaw(yaw);
    node->roll(roll);
    node->pitch(pitch);
}


SceneNode* SceneNodeManager::getNode(IDrawable *object) {
    if (object == NULL) return NULL;

    std::map<IDrawable*,SceneNode*>::const_iterator itr;
    itr = nodes.find(object);
    if (itr == nodes.end()) {
        return NULL;
    } else {
        return itr->second;
    }
}

void SceneNodeManager::deleteNode(IDrawable *object) {
    SceneNode *toDelete = getNode(object);
    if (toDelete != NULL) {
        sceneMgr->destroySceneNode(toDelete);
        nodes.erase(object);
        entities.erase(object);
    }
}

Entity* SceneNodeManager::getEntity(IDrawable *object) {
    std::map<IDrawable*,Entity*>::const_iterator itr;
    itr = entities.find(object);
    if (itr == entities.end()) {
        //Entity has not bee created
        return NULL;
    } else {
        return itr->second;
    }
}

SceneNode* SceneNodeManager::createNode(IDrawable *object) {
    if (getNode(object) != NULL) return getNode(object);

    SceneNode *newNode;
    if (getNode(object->getParentObject()) == NULL) {
        newNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    } else {
        newNode = getNode(object->getParentObject())->createChildSceneNode();
    }
    Entity *newEntity;
    if (!madeShip) {
        newEntity = sceneMgr->createEntity("ourship", object->getMeshName());
        madeShip = true;
    } else {
        newEntity = sceneMgr->createEntity(object->getMeshName());
    }
    //TODO Slight hack ^^
    newNode->attachObject(newEntity);

    entities.insert(std::pair<IDrawable*,Entity*>(object,newEntity));
    nodes.insert(std::pair<IDrawable*,SceneNode*>(object,newNode));

    return newNode;
}

Vector3 SceneNodeManager::rollPitchYawToDirection(Real roll, Real pitch, 
		Real yaw)
{
	Vector3 direction;
	
	// TODO: when pitch and roll actully get used, they may need
	// to be negated or some such.
	Matrix3 rotateX(1,0,0,0,cos(pitch),-sin(pitch),0,sin(pitch),cos(pitch));
	Matrix3 rotateY(cos(-yaw),0,-sin(-yaw),0,1,0,-sin(-yaw),0,cos(-yaw));
	Matrix3 rotateZ(cos(roll),-sin(roll),0,sin(roll),cos(roll),0,0,0,1);
	
	//Matrix3 rotateX(1,0,0,0,cos(pitch),sin(pitch),0,-sin(pitch),cos(pitch));
	//Matrix3 rotateY(cos(yaw),0,-sin(yaw),0,1,0,sin(yaw),0,cos(yaw));
	//Matrix3 rotateZ(cos(roll),sin(roll),0,-sin(roll),cos(roll),0,0,0,1);
	
	//std::cout << "j" << roll << "," << pitch << "," << yaw << "\n";
	
	direction = rotateY * rotateX * rotateZ * Vector3::UNIT_Z;
	
	return direction;
}
