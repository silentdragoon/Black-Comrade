
#ifndef RAY_QUERY_H
#define RAY_QUERY_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"

using namespace Ogre;
using namespace std;

class RayQuery {
private:
    SceneManager *mSceneMgr;
    
    RaySceneQuery *m_pray_scene_query;

    void GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);

public:
    float RaycastFromPoint(const Vector3 &point,
                                        const Vector3 &normal,
                                        Vector3 &result);
    
    RayQuery(SceneManager *mSceneMgr);
};

#endif
