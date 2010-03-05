#ifndef SCENE_NODE_MANAGER_H
#define SCENE_NODE_MANAGER_H

#include <OGRE/Ogre.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIBase.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

#include "ITickable.h"
#include "IDrawable.h"
#include <map>

using namespace Ogre;

class SceneNodeManager : public ITickable
{
    private:
        SceneManager *sceneMgr;
        std::map<IDrawable*, SceneNode*> nodes;
        std::map<IDrawable*, Entity*> entities;
        bool madeShip;

        void updateNode(IDrawable *object, SceneNode *node);

    public:
        SceneNodeManager(SceneManager *sceneMgr);

        SceneNode *createNode(IDrawable *object);
        SceneNode *getNode(IDrawable *object);
        void deleteNode(IDrawable *object);

        Entity *getEntity(IDrawable *object);

        void tick();

        static Vector3 rollPitchYawToDirection(Real roll, Real pitch, 
		Real yaw);
		
		static Vector3 directionToOrientationVector(Vector3 direction);

};

#endif
