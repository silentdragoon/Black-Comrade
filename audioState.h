
#ifndef AUDIO_STATE_H
#define AUDIO_STATE_H

#define OIS_DYNAMIC_LIB
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "gunState.h"
#include "soundManager.h"
#include "const.h"

using namespace Ogre;
using namespace std;

class AudioState : public ITickable
{
    private:
        GunState *gunState;
        SoundManager *sndMgr;
        SceneNode *shipNode;
        
    public:
        
        void tick();
        
        AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode);
        ~AudioState();
};

#endif
