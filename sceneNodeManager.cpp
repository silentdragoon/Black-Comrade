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
