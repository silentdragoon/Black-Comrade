
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                       NotificationManager *notificationMgr, BulletManager *bulletMgr,
                       MiniGameManager *miniGameMgr,GameStateMachine *gameStateMachine)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode),
      notificationMgr(notificationMgr),
      bulletMgr(bulletMgr),
      miniGameMgr(miniGameMgr),
      gameStateMachine(gameStateMachine)
{}

void AudioState::tick()
{
    // Process bullets
    Bullet *b;
    std::vector<Bullet*> *bullets = bulletMgr->getActiveBullets();
   	for(std::vector<Bullet*>::const_iterator it=bullets->begin();it!=bullets->end();++it) {
	    b = *it;
	    if (b->madeNoise) continue;
	    
	    if (b->getOwner()->getEntityType() == ENTT_ENEMY) {
	        // Enemy bullet
	        sndMgr->playSound("sound_enemygun",b->getOrigin(),0.15);
	    } else if (b->getOwner()->getEntityType() == ENTT_PLAYER) {
	        // Player bullet
	        sndMgr->playSound("sound_frontgun",b->getOrigin(),0.6);
	    }
	    
	    b->madeNoise = true;
	}

    if(notificationMgr->hasNewNotification()) {
        Notification *newNotification = notificationMgr->getCurrentNotification();
        if (newNotification->getType() != NT_NONE && newNotification->getSoundNameConst() != "") {
            sndMgr->playSound(newNotification->getSoundNameConst(),shipNode,2);
        }
    }
    
    if(gameStateMachine->isNewState()) {
        GameState state = gameStateMachine->currentGameState();
        switch(state) {
            case GS_STEALTH:
                sndMgr->changeMusic(1);
                break;
            case GS_ATTACK:
                sndMgr->changeMusic(2);
                break;
            case GS_FLEE:
                sndMgr->changeMusic(3);
                break;
            case GS_GAME_OVER:
                sndMgr->changeMusic(4);// TODO: Change to sad music
                break;
            case GS_END:
                sndMgr->changeMusic(4);// TODO: Change to awesome victory music
                break;
        }
    }

    if (miniGameMgr->aKeyPressed) {
        miniGameMgr->aKeyPressed = false;
        sndMgr->playSound("sound_consolekeypress",shipNode,0.1);
    }
}

AudioState::~AudioState() {}
