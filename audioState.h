
#ifndef AUDIO_STATE_H
#define AUDIO_STATE_H

#define OIS_DYNAMIC_LIB
#include <OGRE/Ogre.h>
#include <iostream>
#include "ITickable.h"
#include "gunState.h"
#include "soundManager.h"
#include "const.h"
#include "constManager.h"
#include "bulletManager.h"
#include "notificationManager.h"
#include "miniGameManager.h"

using namespace Ogre;
using namespace std;

class AudioState : public ITickable
{
    private:
        BulletManager *bulletMgr;
        GunState *gunState;
        SoundManager *sndMgr;
        SceneNode *shipNode;
        NotificationManager *notificationMgr;
        MiniGameManager *miniGameMgr;
        
    public:
        
        void tick();
        
        AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                   NotificationManager *notificationMgr, BulletManager *bulletMgr,
                   MiniGameManager *miniGameManager);

        ~AudioState();
};

#endif
