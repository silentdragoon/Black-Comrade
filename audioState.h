
#ifndef AUDIO_STATE_H
#define AUDIO_STATE_H

#define OIS_DYNAMIC_LIB
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "frontGunState.h"
#include "soundManager.h"

using namespace Ogre;
using namespace std;

class AudioState : public ITickable
{
    private:
        FrontGunState *frontGunState;
        SoundManager *sndMgr;
        SceneNode *shipNode;
        
    public:
        
        void tick();
        
        AudioState(FrontGunState *frontGunState, SoundManager *sndMgr, SceneNode *shipNode);
        ~AudioState();
};

#endif
