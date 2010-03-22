
#include "audioState.h"

AudioState::AudioState(GunState *gunState, SoundManager *sndMgr, SceneNode *shipNode,
                       NotificationManager *notificationMgr, BulletManager *bulletMgr,
                       MiniGameManager *miniGameMgr)
    : gunState(gunState),
      sndMgr(sndMgr),
      shipNode(shipNode),
      notificationMgr(notificationMgr),
      bulletMgr(bulletMgr),
      miniGameMgr(miniGameMgr)
{}

void AudioState::tick()
{
    // Process bullets
    Bullet *b;
    std::vector<Bullet*> *bullets = bulletMgr->getActiveBullets();
   	for(std::vector<Bullet*>::const_iterator it=bullets->begin();it!=bullets->end();++it) {
	    b = *it;
	    if (b->madeNoise) continue;
	    
	    if (b->playerStats == 0) {
	        // Enemy bullet
	        sndMgr->playSound(ConstManager::getInt("sound_enemygun"),b->getOrigin(),0.2);
	    } else {
	        // Player bullet
	        sndMgr->playSound(ConstManager::getInt("sound_frontgun"),b->getOrigin(),0.8);
	    }
	    
	    b->madeNoise = true;
	}

    if(notificationMgr->hasNewNotification()) {
        Notification *newNotification = notificationMgr->getCurrentNotification();
        if (newNotification->getType() != NT_NONE && newNotification->getSoundNameConst() != -1) {
            sndMgr->playSound(newNotification->getSoundNameConst(),shipNode,2);
        }
        if (newNotification->getMusic() != -1) {
            std::cout << "MUSIC: " << newNotification->getMusic() << std::endl;
            sndMgr->changeMusic(newNotification->getMusic());
        }
    }

    if (miniGameMgr->aKeyPressed) {
        miniGameMgr->aKeyPressed = false;
        sndMgr->playSound(ConstManager::getInt("sound_consolekeypress"),shipNode,0.1);
    }
}

AudioState::~AudioState() {}
