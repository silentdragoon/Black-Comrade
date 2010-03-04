
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
    if(bulletMgr->playerFire) {
        sndMgr->playSound(ConstManager::getInt("sound_frontgun"),shipNode,shipNode,0.5,true);
    }

    if(bulletMgr->enemyFire) {
        sndMgr->playSound(ConstManager::getInt("sound_enemygun"),shipNode,bulletMgr->enemyNode,0.3,true);
    }

    if(notificationMgr->hasNewNotification()) {
        Notification *newNotification = notificationMgr->getCurrentNotification();
        if (newNotification->getType() != NT_NONE && newNotification->getSoundNameConst() != -1) {
            sndMgr->playSound(newNotification->getSoundNameConst(),shipNode,shipNode,2,true);
        }
    }

    if (miniGameMgr->aKeyPressed) {
        miniGameMgr->aKeyPressed = false;
        sndMgr->playSound(ConstManager::getInt("sound_consolekeypress"),shipNode,shipNode,0.1,true);
    }
}

AudioState::~AudioState() {}
