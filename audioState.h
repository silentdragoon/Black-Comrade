
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
#include "gameStateMachine.h"
#include "miniGameManager.h"
#include "damageState.h"
#include "objective.h"
#include <vector>
#include "bullet.h"

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
        GameStateMachine *gameStateMachine;
        Objective *objective;
        DamageState *damageState;

        int prevTime;
        int tickTime;

        bool playedExplosion;
        
    public:
        
        void tick();
        
        AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                   NotificationManager *notificationMgr, BulletManager *bulletMgr,
                   MiniGameManager *miniGameManager, GameStateMachine *gameStateMachine,
                   Objective *objective, DamageState *damageState);

        ~AudioState();
};

#endif
